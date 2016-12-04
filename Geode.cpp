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