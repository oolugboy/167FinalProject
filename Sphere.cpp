#include "Sphere.h"
#include "Camera.h"
#include <cmath>

using namespace std;

GLuint Sphere::textureID;
GLuint toWorldLoc;
GLuint cam_posLoc;
Sphere::Sphere(float radius, bool wired)
{	
	/* Set the number of longitudes and latitudes */
	latNum = 30;
	longNum = 30;
	/* Do not use the radius yet */
	m_radius = 1.0f;
	this->wired = wired;
	currPos = glm::vec3(0, 0, 0);	
	
	/* generate the sphere data */
	genVertices();
	genIndices();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &NBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 /* Now to load the normals */
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// We've sent the vertex and normal data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
void Sphere::genVertices()
{
	for (int i = 0; i <= latNum; i++)
	{
		float theta = (i * PI) / latNum;

		for (int j = 0; j <= longNum; j++)
		{
			float phi = (j * 2 * PI) / longNum;
			float x = m_radius * sin(theta) * cos(phi);
			float y = m_radius * cos(theta);
			float z = m_radius * sin(theta) * sin(phi);

			glm::vec3 newPos = glm::vec3(x, y, z);
			vertices.push_back(newPos);
			normals.push_back(glm::normalize(newPos));
		}
	}
}

void Sphere::genIndices()
{
	for (int i = 0; i < latNum; i++)
	{
		for (int j = 0; j < longNum; j++)
		{
			int first = (i  * (longNum + 1)) + j;
			int second = first + (longNum + 1);
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}
}
void Sphere::setUpFrameBuffer()
{
	/* We generate the frame buffer */
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	/* First we define the depth texture  */
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* We then attach the texture as the frame buffer's depth buffer */
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Sphere::depthMapRender()
{
	/* Adjust the resolution for the shadow maps */
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	/*ConfigureShaderAndMatrices();
	RenderScene();*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
bool Sphere::inBound()
{	
	return true;
}
void Sphere::render()
{
	// Calculate the combination of the model and view (camera inverse) matrices
	modelView = Window::V * toWorld;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(Window::agentShaderProgram, "projection");
	uModelview = glGetUniformLocation(Window::agentShaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelView[0][0]);


	toWorldLoc = glGetUniformLocation(Window::agentShaderProgram, "model");
	glUniformMatrix4fv(toWorldLoc, 1, GL_FALSE, &(toWorld[0][0]));

	cam_posLoc = glGetUniformLocation(Window::agentShaderProgram, "cam_pos");
	glUniform3fv(cam_posLoc, 1, &(Window::camera->cam_pos[0]));


	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	/*glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(Window::agentShaderProgram, "texBox"), 0);*/
	// Now to bind the skybox texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glEnable(GL_DEPTH_TEST);

	//glUniform1i(glGetUniformLocation(Window::shaderProgram2, "isTexture"), 0);

	/* figure out the width of the lines*/
	if (wired)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}	
	
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);	
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//TODO: remember to unbind the texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}