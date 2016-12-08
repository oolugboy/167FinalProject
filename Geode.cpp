#include "Geode.h"
#include <iostream>

using namespace std;

Geode::Geode()
{	
	currPos = glm::vec3(0.0f, 0.0f, 0.0f);
	toWorld = glm::mat4(1.0f);
	scaleMult = 1.0f;	
	xBounds = 1.0f;
	yBounds = 1.0f;
	zBounds = 1.0f;
	xWidth = 2;
	yWidth = 2;
	zWidth = 2;	
}
void Geode::draw(glm::mat4 cMatrix)
{	
	/* Then render the program */
	toWorld = cMatrix;
	
	// Calculate the combination of the model and view (camera inverse) matrices
	modelView = Window::V * toWorld;

	/* Store the position of the object in camera space */
	glm::vec4 temp = toWorld[3];
	temp = modelView * temp;	

	currPos = glm::vec3(temp.x, temp.y, temp.z);
	render();
}
void Geode::update()
{	
	
}

GLuint Geode::loadTexture(GLchar* path)
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