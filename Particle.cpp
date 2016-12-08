#include "Particle.h"
#include <iostream>


ParticleGenerator::ParticleGenerator(Shader shader, GLuint amount)
	: shader(shader), amount(amount)
{
	this->init();
	texture = loadTexture("textures/container.jpg");
}

void ParticleGenerator::Update(GLfloat dt, Ball &object, GLuint newParticles, glm::vec2 offset)
{
	// Add new particles 
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

// Render all particles
void ParticleGenerator::Draw()
{
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			glUniform2f(glGetUniformLocation(this->shader.Program, "offset"), particle.Position.x, particle.Position.y);
			glUniform4f(glGetUniformLocation(this->shader.Program, "color"), particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);
			glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(Window::V));
			glUniform1i(glGetUniformLocation(this->shader.Program, "particle"), 0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, Ball &object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = glm::vec2(object.currPos.x, object.currPos.z) + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec2(object.initVelocity.x, object.initVelocity.z) * 0.1f;
}

GLuint ParticleGenerator::loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}












//
//using namespace std;
//
//Particle::Particle()
//{	
//	generateVAO();
//}
//
//Particle::Particle(glm::vec3 pos, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotAngle, float scale)
//{
//	this->pos = glm::vec3(pos);
//	this->o_pos = glm::vec3(pos);
//	this->velocity = velocity;
//	this->gravityEffect = gravityEffect;
//	this->lifeLength = lifeLength;
//	this->rotAngle = rotAngle;
//	this->scale = scale;
//	generateVAO();
//
//	elapsedTime = 0.0f;
//}
//
//Particle::~Particle()
//{
//	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
//	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//	glDeleteBuffers(1, &TBO);
//}
//
//void Particle::generateVAO()
//{// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	glGenBuffers(1, &TBO);
//
//	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
//	// Consider the VAO as a container for all your buffers.
//	glBindVertexArray(VAO);
//
//	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
//	// you want to draw, such as vertices, normals, colors, etc.
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
//	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertNum, vertices, GL_STATIC_DRAW);
//	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
//		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
//		GL_FLOAT, // What type these components are
//		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
//		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
//		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
//
//					 /* Now to load the normals */
//	glBindBuffer(GL_ARRAY_BUFFER, TBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_v[0]) * textNum, texture_v, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//
//	// We've sent the vertex and normal data over to OpenGL, but there's still something missing.
//	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	// Unbind the VAO now so we don't accidentally tamper with it.
//	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
//	glBindVertexArray(0);
//
//	particleShader = new Shader("shaders/city.vert", "shaders/city.frag");
//}
//
//void Particle::update()
//{
//	float time_diff = float(clock() - time) / CLOCKS_PER_SEC;
//	float x = elapsedTime / lifeLength;
//	float y = -4 * (x - 1) * (x - 1) + 4;
//	pos = o_pos + glm::vec3(x, y, 0.0);
//
//	elapsedTime += time_diff;
//	printf(" Particle update: (new pos = (%f, %f, %f)\n", pos.x, pos.y, pos.z);
//	printf(" Particle update: (diff = %f, r_time=%t, l=%t)\n", time_diff, velocity.y, (float)elapsedTime, (float)lifeLength);
//}
//
//bool Particle::isLive() {
//	return elapsedTime < lifeLength;
//}
//
//void Particle::render()
//{	
//	particleShader->Use();
//	glm::mat4 translate = glm::translate(glm::mat4(1.0f), pos);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale));
//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotAngle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
//	glm::mat4 modelview = Window::V * toWorld * translate * rot * scale;
//
//	glUniformMatrix4fv(glGetUniformLocation(particleShader->Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
//	glUniformMatrix4fv(glGetUniformLocation(particleShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));
//	glBindVertexArray(VAO);
//
//	// Now draw the Particle. We simply need to bind the VAO associated with it.
//	glEnable(GL_DEPTH_TEST);
//	if (wired)
//	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	}
//	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
//	glBindVertexArray(0);
//
//	printf("Particle toWorld: (%f,%f,%f) \n", toWorld[3].x, toWorld[3].y, toWorld[3].z);
//}
//
