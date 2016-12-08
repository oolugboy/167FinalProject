#include "Building.h"
#include <cmath>

GLuint Building::playerTexID;
GLuint Building:: aITexID;
Building::Building(bool isPlayer, glm::vec3 position, MatrixTransform * matrixT)
{
	this->prevPos = this->currPos = this->initPos = position;
	this->isPlayer = isPlayer;
	maxSpeed = 0.01f;
	movable = false;

	/* The velocity values */
	initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);

	/** Bounds for the collision detection */
	xBounds = 0.0f, yBounds = 0.0f, zBounds = 0.0f;
	xWidth = 0.0f, yWidth = 0.0f, zWidth = 0.0f;

	/* These are the objects relative x,y, and z coordinates */
	this->BuildingX = glm::vec3(1.0f, 0.0f, 0.0f);
	this->BuildingY = glm::vec3(0.0f, 1.0f, 0.0f);
	this->BuildingZ = glm::vec3(0.0f, 0.0f, 1.0f);

	this->direction = -1.0f * BuildingZ;

	/* Set arbitatry value for the mass and acceleration */
	mass = 99999999.0f;
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	t = 0.0f;
	inAir = false;
	turn = 1;

	this->matrixT = matrixT;
}

Building::~Building() {
	cout << "delete building" << endl;
	delete(matrixT);
}
void Building::accelerate(bool posAccel)
{
}

void Building::update()
{	
	updateBoxVals();
}

void Building::draw(glm::mat4 cMatrix) {
	matrixT->draw(cMatrix);
}


void Building::updateBoxVals()
{
	glm::vec4 temp = matrixT->transformMatrix[3];
	xBounds = (1.0f * matrixT->transformMatrix[0][0]) + temp.x;
	yBounds = (1.0f * matrixT->transformMatrix[1][1]) + temp.y;
	zBounds = (1.0f * matrixT->transformMatrix[2][2]) + temp.z;
	xWidth = 2.0f * matrixT->transformMatrix[0][0];
	yWidth = 2.0f * matrixT->transformMatrix[1][1];
	zWidth = 2.0f * matrixT->transformMatrix[2][2];
}
bool Building::outOfBounds()
{
	if (abs(this->currPos.x) > 50.0f)
		return true;
	if (abs(this->currPos.z > 50.0f))
		return true;
	return false;
}

bool Building::collidesWith(Object * otherObject)
{	
	float xRMax = fmax(xBounds, otherObject->xBounds);
	float xLMin = fmin(xBounds - xWidth, otherObject->xBounds - otherObject->xWidth);

	if (abs(xRMax - xLMin) < (xWidth + otherObject->xWidth))
	{
		float yRMax = fmax(yBounds, otherObject->yBounds);
		float yLMin = fmin(yBounds - otherObject->yWidth, otherObject->yBounds - otherObject->yWidth);
		if (abs(yRMax - yLMin) < (yWidth + otherObject->yWidth))
		{	
			float zRMax = fmax(zBounds, otherObject->zBounds);
			float zLMin = fmin(zBounds - zWidth, otherObject->zBounds - otherObject->zWidth);
			if (abs(zRMax - zLMin) < (zWidth + otherObject->zWidth))
			{
				return true;
			}
		}
	}
	return false;
}
void Building::handleCollision(Object * otherObject)
{	/*
	if (isPlayer)
		cout << " The player handling collision " << endl;
	else
		cout << " The other handling collision " << endl;	
	
	if (isPlayer)
	{
		cout << " The players initial pre collision velocity is ";
		printVector(initVelocity);
	}*/
	
	glm::vec3 collideVector = glm::normalize(otherObject->currPos - currPos);
	if (glm::dot(collideVector, initVelocity) > 0.0f)
	{
		
	}
	initVelocity = finalVelocity;
	float xSpeed = (((initVelocity.x  * (this->mass - otherObject->mass)) + (2.0f * otherObject->mass * otherObject->initVelocity.x)) / (this->mass + otherObject->mass));
	float zSpeed = (((initVelocity.z  * (this->mass - otherObject->mass)) + (2.0f * otherObject->mass * otherObject->initVelocity.z)) / (this->mass + otherObject->mass));

	finalVelocity = glm::vec3(xSpeed, 0.0f, zSpeed);
	matrixT->transformMatrix = glm::translate(glm::mat4(1.0f), finalVelocity * 600.0f) * matrixT->transformMatrix;	

	acceleration = (finalVelocity - initVelocity)/100.0f;
	t = clock();

	/** Update the acceleration */
	/*if (isPlayer == false)
	{
		cout << " The aI post collision final velocity is ";
		printVector(finalVelocity);
	}
	else
	{
		cout << " The player post collision final velocity is ";
		printVector(finalVelocity);
		cout << " While the aI post collision final velocity is ";
		printVector(otherObject->finalVelocity);
	}*/

}



