#include "Ball.h"
#include <cmath>

GLuint Ball::playerTexID;
GLuint Ball:: aITexID;
Ball::Ball(bool isPlayer, glm::vec3 position, MatrixTransform * matrixT)
{	
	this->prevPos = this->currPos = this->initPos = position;
	this->isPlayer = isPlayer;		
	maxSpeed = 0.01f;
	movable = true;

	/* The velocity values */
	initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);

	/** Bounds for the collision detection */
	xBounds = 0.0f, yBounds = 0.0f, zBounds = 0.0f;
	xWidth = 0.0f, yWidth = 0.0f, zWidth = 0.0f;

	/* These are the objects relative x,y, and z coordinates */
	this->ballX = glm::vec3(1.0f, 0.0f, 0.0f);
	this->ballY = glm::vec3(0.0f, 1.0f, 0.0f);
	this->ballZ = glm::vec3(0.0f, 0.0f, 1.0f);	

	this->direction = -1.0f * ballZ;

	/* Set arbitatry value for the mass and acceleration */
	mass = 2.5f;
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	t = 0.0f;
	inAir = false;
	turn = 1;

	this->matrixT = matrixT;
	this->matrixT->transformMatrix = glm::translate(glm::mat4(1.0f), position);
}

void Ball::jump(bool accel)
{
	//cout << " We are jumping " << endl;
	inAir = true;
	glm::vec3 upwardForce = (0.4f * ballY);
	if (accel)
		forwardForce = (-0.00100f * ballZ);
	else
		forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);
	/* Make sure the acceleration direction is updated */
	float a_mag = getMag(acceleration);
	acceleration = a_mag * direction;

	acceleration = ((mass * acceleration) + forwardForce + upwardForce) / mass;
}

void Ball::accelerate(bool posAccel)
{
	glm::vec3 frictionalForce = (0.000001f * (-1.0f * direction));
	glm::vec3 gravitationalForce = glm::vec3(0.0f, 0.0f, 0.0f);

	if (getMag(initVelocity) == 0.0f)
	{
		frictionalForce = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (posAccel) 
	{
		forwardForce = (-0.0003f * ballZ);
		acceleration = (forwardForce + frictionalForce + gravitationalForce) / mass;
	}
	else
	{
		forwardForce = glm::vec3(0.0f);
		acceleration = ((mass * acceleration) + frictionalForce + gravitationalForce) / mass;
	}

	/** Update the direction of the acceleration */
	float a_mag = getMag(acceleration);
	acceleration = a_mag * direction;
	
	//acceleration = ((mass * acceleration) + forwardForce + frictionalForce + gravitationalForce) / mass;
	
	//acceleration = (turn != 1) ? acceleration * 0.5f : acceleration;
	acceleration = (turn != 1) ? acceleration  : acceleration;
	if (isPlayer)
	{
		//cout << " The just done acceleration is ";
		//printVector(acceleration);
	}
}
void Ball::update()
{
	clock_t temp = clock();
	//cout << " The temp is " << temp - t << endl;
	currPos = glm::vec3(matrixT->transformMatrix[3]);

	if (temp - t > 0.0002) {
		checkMaxSpeed();
		if (!getCollided)
		{
			updateVelocity((float)temp - t);
		}
		t = temp;
	}

		if (isPlayer)
		{
			//cout << "FALL" << endl;
			//cout << " The currPos is ";
			//printVector(currPos);
		}		
}
void Ball::draw(glm::mat4 cMatrix) {
	glUseProgram(Window::agentShaderProgram);
	if (isPlayer)
		Sphere::textureID = playerTexID;
	else
		Sphere::textureID = aITexID;
	matrixT->draw(cMatrix);
}

/* This function updates the initial and the final velocity */
void Ball::updateVelocity(float sec)
{
//	cout << " About to update " << endl;
	/* Update the object's directional attributes if the object is moving*/
	if (currPos != prevPos)
	{
		glm::vec3 diff = currPos - prevPos;
		direction = glm::normalize(glm::vec3(diff.x, 0.0f, diff.z));
		if (inAir)
			direction = glm::normalize(glm::vec3(diff.x, diff.y, diff.z));

	/*	cout << " The direction is ";
		printVector(direction); */
		ballZ = (-1.0f * direction);
		ballX = glm::normalize(cross(ballY, ballZ));		
		/*cout << " The acceleration is ";
		printVector(acceleration);*/
	}
	if (!(isPlayer))
	{
		//cout << " The other player's former final velocity is ";
		//printVector(finalVelocity);		
	}	
	/*Update the velociy */
	if (isPlayer)
	{
		/*printf("Begin of Calculation %d, (%f,%f,%f), |%f|, (%f,%f,%f), |%f|\n", inAir,
			finalVelocity.x, finalVelocity.y, finalVelocity.z, getMag(finalVelocity),
			initVelocity.x, initVelocity.y, initVelocity.z, getMag(initVelocity));*/
	}
	initVelocity = finalVelocity;


	/* The stopping condition */
	/*if (getMag(initVelocity) < 0.5 && getMag(acceleration) < 0.0001) {
		initVelocity = finalVelocity = acceleration = glm::vec3(0.0f);
	} */
	//else {
		finalVelocity = initVelocity + (acceleration * sec);
	//}

	/* For the turning */
	glm::vec3 turnForce = glm::vec3(0.0f);
	glm::vec3 turnVelocity = glm::vec3(0.0f);
	if (turn == 0) { // turn left
		turnForce = (-0.001000f * ballX) / mass;
		turnVelocity = (sec* turnForce / mass);
	}
	else if (turn == 2) { //turn right
		turnForce = (0.00100f * ballX) / mass;
		turnVelocity = (sec* turnForce / mass);
	}
	if (turn != 1)
	{
		/* Update the appropriate values in case of a turn */
		finalVelocity = (finalVelocity + turnVelocity) * 0.70f;
		//acceleration = (finalVelocity - initVelocity) / sec;
	}
	if (inAir)
	{
		finalVelocity = (finalVelocity + glm::vec3(0.0f, -0.005f, 0.0f));
	}
	turn = 1; //reset to no turn


	if (isPlayer)
	{
		/*printf("End of Calculation %d (%f,%f,%f), |%f|, (%f,%f,%f), |%f|\n", inAir,
			finalVelocity.x, finalVelocity.y, finalVelocity.z, getMag(finalVelocity),
			initVelocity.x, initVelocity.y, initVelocity.z, getMag(initVelocity));*/
		//cout << " The speed is " << getMag(finalVelocity) << endl;
		//cout << " The finalVelocity  is ";
		//printVector(finalVelocity);
	}
	else
	{		
		//cout << " The other player's updated acceleration is ";
		//printVector(acceleration);
	}

	prevPos = currPos;
	updateBoxVals();

	/* The move the agent */
	moveAgent(sec);
}
void Ball::updateBoxVals()
{
	glm::vec4 temp = matrixT->transformMatrix[3];
	xBounds = (1.0f * matrixT->transformMatrix[0][0]) + temp.x;
	yBounds = (1.0f * matrixT->transformMatrix[1][1]) + temp.y;
	zBounds = (1.0f * matrixT->transformMatrix[2][2]) + temp.z;
	xWidth = 2.0f * matrixT->transformMatrix[0][0];
	yWidth = 2.0f * matrixT->transformMatrix[1][1];
	zWidth = 2.0f * matrixT->transformMatrix[2][2];
}
bool Ball::outOfBounds()
{
	if (abs(this->currPos.x) > 50.0f)
		return true;
	if (abs(this->currPos.z > 50.0f))
		return true;
	return false;
}
void Ball::checkMaxSpeed()
{
	if (getMag(finalVelocity) > maxSpeed)
	{
		finalVelocity = glm::normalize(finalVelocity) * maxSpeed;
	}
}
float Ball::getMag(glm::vec3 dir)
{
	return sqrt(pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2));
}
void Ball::moveAgent(float sec)
{
	glm::vec3 diff =(initVelocity * sec) + (0.50f * acceleration * pow(sec, 2));	
	
	/*cout << " The diff is ";
	printVector(diff);*/
	matrixT->transformMatrix = glm::translate(glm::mat4(1.0f), diff) * matrixT->transformMatrix;
}
bool Ball::collidesWith(Object * otherObject)
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
void Ball::handleCollision(Object * otherObject)
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
	matrixT->transformMatrix = glm::translate(glm::mat4(1.0f), finalVelocity * 200.0f) * matrixT->transformMatrix;	

	acceleration = (finalVelocity - initVelocity)/500.0f;
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
glm::vec3 Ball::cross(glm::vec3 a, glm::vec3 b)
{
	float xVal = (a.y * b.z) - (a.z * b.y);
	float yVal = ((a.x * b.z) - (a.z * b.x)) * -1.0f;
	float zVal = (a.x * b.y) - (a.y * b.x);
	return glm::vec3(xVal, yVal, zVal);
}
// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint Ball::loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL)
	{
		cout << " We could not load the image at " << path << " with width " << width << endl;
	}
	else
	{
		cout << " We just loaded the texture at " << path << " with width " << width << endl;
	}
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



