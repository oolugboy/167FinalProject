#include "City.h"
#include "Window.h"


City::City()
{
	this->world_intervals = 10;
	for (int z = 0; z < world_intervals; z++) {
		std::vector<bool> horizonal;
		for (int x = 0; x < world_intervals; x++) {
			horizonal.push_back(false);
		}
		this->occupation.push_back(horizonal);
	}
	computePoints();
	toWorld = glm::mat4(1.0f);

	occupationShader = new Shader("../shaders/city_occupation.vert", "../shaders/city_occupation.frag");
}

City::~City()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &EBO);
}

GLuint City::generateVAO() {
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texture_v.size() * sizeof(glm::vec2), &(texture_v[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &(indices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	texture = loadTexture("../textures/glass-brick.jpg");

	return VAO;
}

bool City::addObject(Node* object, glm::mat4 drawMatrix) {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	occupationShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(occupationShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(toWorld));
	glUniformMatrix4fv(glGetUniformLocation(occupationShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(Window::V));
	glUniformMatrix4fv(glGetUniformLocation(occupationShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));

	glUniform1i(glGetUniformLocation(occupationShader->Program, "code"), 1);

	object->draw(drawMatrix);

	glm::mat4 pc = Window::P * Window::V;
	unsigned char res[4];
	for (int z = 0; z < world_intervals; z++) {
		for (int x = 0; x < world_intervals; z++) {
			glm::vec3 position = getPosition(x, z);
			glm::vec4 p = pc * glm::vec4(position, 1.0);
			float xpos = p.x / p.w;
			float ypos = p.y / p.w;
			printf("(x,z) = (%d,%d) = (%f,%f)\n", x, z, xpos, ypos);
		}
	}
	//glReadPixels(xpos, Window::height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	//std::cout << "Select control point (" << (unsigned int)res[0] << "," << (unsigned int)res[1] << "," << (unsigned int)res[2] << "," << (unsigned int)res[3] << ")" << std::endl;

}

void City::draw(Shader shader)
{
	glm::mat4 modelview = Window::V * toWorld;

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::P));
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.Program, "tile"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//glLineWidth(3.0f);
	//glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glPointSize(10.0f);
	//glDrawArrays(GL_POINTS, 0, vertices.size());
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
	//glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glBindVertexArray(0);
}


void City::drawCurves(Shader shader)
{
	for (int i = 0; i < curves_v.size(); i++) {
		curves_v[i]->draw(shader);
	}


	for (int i = 0; i < curves_h.size(); i++) {
		curves_h[i]->draw(shader);
	}
}


void City::computePoints() {
	this->generateCurves();

	int z = 1;
	glm::vec3 v_pos, h_pos, pos0, pos1, pos2, pos3;
	int count = 0;
	for (int z = 0; z < world_intervals; z++) {
		for (int x = 0; x < world_intervals; x++) {
			v_pos = curves_v[world_intervals - x]->getPosition(z);
			h_pos = curves_h[world_intervals - z]->getPosition(x);
			pos0 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos0);
			texture_v.push_back(glm::vec2(0.0, 1.0));

			v_pos = curves_v[world_intervals - x]->getPosition(z + 1);
			h_pos = curves_h[world_intervals - z - 1]->getPosition(x);
			pos1 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos1);
			texture_v.push_back(glm::vec2(0.0, 0.0));

			v_pos = curves_v[world_intervals - x - 1]->getPosition(z);
			h_pos = curves_h[world_intervals - z]->getPosition(x + 1);
			pos2 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos2);
			texture_v.push_back(glm::vec2(1.0, 1.0));

			vertices.push_back(pos2);
			texture_v.push_back(glm::vec2(1.0, 1.0));
			vertices.push_back(pos1);
			texture_v.push_back(glm::vec2(0.0, 0.0));

			v_pos = curves_v[world_intervals - x - 1]->getPosition(z + 1);
			h_pos = curves_h[world_intervals - z - 1]->getPosition(x + 1);
			pos3 = (v_pos + h_pos) * 0.5f;
			vertices.push_back(pos3);
			texture_v.push_back(glm::vec2(1.0, 0.0));

			//std::cout << "z=" << z << ", x=" << x << "th vertices: (" << pos.x << "," << pos.y << "," << pos.z << ")" << std::endl;
			//std::cout << "z=" << z << ", x=" << x << "th vertices: (" << v_pos.x << "," << v_pos.y << "," << v_pos.z << ")" << std::endl;
			//std::cout << "z=" << world_intervals - z << ", x=" << x << "th vertices: (" << h_pos.x << "," << h_pos.y << "," << h_pos.z << ")" << std::endl;
		}
	}


	for (int z = 0; z <= world_intervals; z++) {
		for (int x = 0; x <= world_intervals; x++) {
			glm::vec3 v_pos = curves_v[world_intervals - x]->getPosition(world_intervals - z);
			glm::vec3 h_pos = curves_h[z]->getPosition(x);
			glm::vec3 pos = (v_pos + h_pos) * 0.5f;
			city_grid.push_back(pos);
			printf("(x,z) = (%d,%d) = (%f,%f)\n", x, z, pos.x, pos.z);
		}
	}

	glm::mat4 pc = Window::P * Window::V;
	for (int z = 0; z <= world_intervals; z++) {
		for (int x = 0; x <= world_intervals; x++) {
			glm::vec3 position = getPosition(x, z);
			glm::vec4 p = pc * glm::vec4(position, 1.0);
			float xpos = (Window::width * p.x / p.w + Window::width) / 2.0;
			float ypos = (Window::height * p.y / p.w + Window::height) / 2.0;


			printf("(x,z) = (%d,%d) = (%f,%f) = (%f,%f)\n", x, z, position.x, position.z, xpos, ypos);
		}
	}

	//lines index
	//for (int z = 0; z <= world_intervals; z++) {
	//	for (int x = 0; x <= world_intervals; x++) {
	//		if (x != world_intervals) {
	//			indices.push_back(world_intervals*z + z + x);
	//			indices.push_back(world_intervals*z + z + x + 1);
	//		}
	//		if (z != world_intervals) {
	//			indices.push_back(world_intervals*z + z + x);
	//			indices.push_back(world_intervals*(z + 1) + z + 1 + x);
	//		}
	//	}
	//}
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(world_intervals + 1);
	//indices.push_back(world_intervals + 2);

	//for (int i = 0; i < vertices.size() - 1; i++) {
	//	indices.push_back(i);
	//	indices.push_back(i + 1);
	//}
	
}

void City::generateCurves() {

	int half = world_intervals / 2;
	int left = -1 * world_intervals / 2;
	int right = world_intervals / 2 + world_intervals % 2;

	float x_diff1, x_diff2, z_diff1, z_diff2;
	int x1_set_point = - half / 6;
	int x1_left = abs(x1_set_point - left);
	int x1_right = abs(x1_set_point - right);
	int x2_set_point = 3 * half / 5;
	int x2_left = abs(x2_set_point - left);
	int x2_right = abs(x2_set_point - right);
	int z1_set_point = -4 * half / 7;
	int z1_left = abs(z1_set_point - left);
	int z1_right = abs(z1_set_point - right);
	int z2_set_point = 2 * half / 5;
	int z2_left = abs(z2_set_point - left);
	int z2_right = abs(z2_set_point - right);

	//printf("Getting vertical lines\n");
	for (int i = 0; i <= world_intervals; i++) {
		int x = i - world_intervals / 2;
		//printf("  Getting vertical lines %d on x = %d\n", i, x);
		float z = (world_intervals / 4.0);
		float x1 = x - x1_set_point;
		float x2 = x - x2_set_point;
		if (x <= x1_set_point)
			x_diff1 = (world_intervals / 5.0) * (-1.0 * x1 * x1 + x1_left * x1_left) / (float)(x1_left * x1_left);
		else
			x_diff1 = (world_intervals / 5.0) * (-1.0 * x1 * x1 + x1_right * x1_right) / (float)(x1_right * x1_right);
		if (x <= x2_set_point)
			x_diff2 = (world_intervals / 3.0) * (-1.0 * x2 * x2 + x2_left * x2_left) / (float)(x2_left * x2_left);
		else
			x_diff2 = (world_intervals / 3.0) * (-1.0 * x2 * x2 + x2_right * x2_right) / (float)(x2_right * x2_right);

		float z1 = z - z1_set_point;
		float z2 = z - z2_set_point;
		if (z <= z1_set_point)
			z_diff1 = (world_intervals / 2.0) * (-1.0 * z1 * z1 + z1_left * z1_left) / (float)(z1_left * z1_left);
		else
			z_diff1 = (world_intervals / 2.0) * (-1.0 * z1 * z1 + z1_right * z1_right) / (float)(z1_right * z1_right);
		if (z <= x2_set_point)
			z_diff2 = (world_intervals / 4.0) * (-1.0 * z2 * z2 + z2_left * z2_left) / (float)(z2_left * z2_left);
		else
			z_diff2 = (world_intervals / 4.0) * (-1.0 * z2 * z2 + z2_right * z2_right) / (float)(z2_right * z2_right);

		std::vector<glm::vec3> cp = { { x, 0.0, right },{ x - x_diff1, 0.0, z + z_diff1 }  ,{ x + x_diff2, 0.0, -1 * z - z_diff2 }  ,{ x, 0.0, left } };  //vertical
		Curve* curve = new Curve(&cp, world_intervals);
		curve->generateVAO();
		curves_v.push_back(curve);
	}

	//printf("Getting horizontal lines\n");
	for (int i = 0; i <= world_intervals; i++) {
		int z = i - world_intervals / 2;
		//printf("  Getting horizontal lines %d on z = %d\n", i, z);
		float x = (world_intervals / 4.0);
		float x1 = x - x1_set_point;
		float x2 = x - x2_set_point;
		if (x <= x1_set_point)
			x_diff1 = (world_intervals / 3.0) * (-1.0 * x1 * x1 + x1_left * x1_left) / (float)(x1_left * x1_left);
		else
			x_diff1 = (world_intervals / 3.0) * (-1.0 * x1 * x1 + x1_right * x1_right) / (float)(x1_right * x1_right);
		if (x <= x2_set_point)
			x_diff2 = (world_intervals / 1.5) * (-1.0 * x2 * x2 + x2_left * x2_left) / (float)(x2_left * x2_left);
		else
			x_diff2 = (world_intervals / 1.5) * (-1.0 * x2 * x2 + x2_right * x2_right) / (float)(x2_right * x2_right);

		float z1 = z - z1_set_point;
		float z2 = z - z2_set_point;
		if (z <= z1_set_point)
			z_diff1 = (world_intervals / 3) * (-1.0 * z1 * z1 + z1_left * z1_left) / (float)(z1_left * z1_left);
		else
			z_diff1 = (world_intervals / 3) * (-1.0 * z1 * z1 + z1_right * z1_right) / (float)(z1_right * z1_right);
		if (z <= x2_set_point)
			z_diff2 = (world_intervals / 10.0) * (-1.0 * z2 * z2 + z2_left * z2_left) / (float)(z2_left * z2_left);
		else
			z_diff2 = (world_intervals / 10.0) * (-1.0 * z2 * z2 + z2_right * z2_right) / (float)(z2_right * z2_right);

		std::vector<glm::vec3> cp = { { right, 0.0, z },{ x - x_diff1, 0.0, z + z_diff1 }  ,{ -1 * x + x_diff2, 0.0, z - z_diff2 }  ,{ left, 0.0, z } };  //horizontal
		Curve* curve = new Curve(&cp, world_intervals);
		curve->generateVAO();
		curves_h.push_back(curve);
	}
}

glm::vec3 City::getPosition(float t) {

	glm::vec3 position;
	//int size = (int)(1.0f / step) * 3;
	//int j = (int)(t * size);
	//int r = j % 3;
	//j -= r;
	//position = vertices[(int)(t / step)];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}

glm::vec3 City::getPosition(int x, int z) {

	if (x > world_intervals || z > world_intervals) {
		return glm::vec3(-10000, -10000, -10000);
	}

	glm::vec3 position;
	//int j = index * 3;
	position = city_grid[world_intervals*z + z + x];
	//position.x = vertices[j];
	//position.y = vertices[j + 1];
	//position.z = vertices[j + 2];
	return position;
}



// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint City::loadTexture(GLchar* path)
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
