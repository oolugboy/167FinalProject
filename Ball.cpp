#include "Ball.h"
#include <cmath>

using namespace std;


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
	cout << " We are jumping " << endl;
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

	/* If the ball is in the air */
	if (inAir)
	{
		gravitationalForce = (-0.003f * glm::vec3(0.0f, 1.0f, 0.0f));
		frictionalForce = forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);
		if (currPos.y < 0.01f)/* Offset from the ground */
		{
			/* Glide the ball back to the ground*/
			//forwardForce = 
		}
	}

	/** Update the direction of the acceleration */
	float a_mag = getMag(acceleration);
	acceleration = a_mag * direction;
	
	//acceleration = ((mass * acceleration) + forwardForce + frictionalForce + gravitationalForce) / mass;
	
	//acceleration = (turn != 1) ? acceleration * 0.5f : acceleration;
	acceleration = (turn != 1) ? acceleration  : acceleration;
	cout << " The acceleration is ";
	printVector(acceleration);

}
void Ball::update()
{
	clock_t temp = clock();
	cout << " The temp is " << temp - t << endl;
	currPos = glm::vec3(matrixT->transformMatrix[3]);

	if (temp - t > 0.0002) {
		checkMaxSpeed();
		if (!getCollided)
		{
			updateVelocity((float)temp - t);
		}
		t = temp;
	}
}


void Ball::draw(glm::mat4 cMatrix) {
	matrixT->draw(cMatrix);
}

/* This function updates the initial and the final velocity */
void Ball::updateVelocity(float sec)
{
	/* Update the object's directional attributes if the object is moving*/
	if (currPos != prevPos)
	{
		glm::vec3 diff = currPos - prevPos;
		direction = glm::normalize(glm::vec3(diff.x, 0.0f, diff.z));

		cout << " The direction is ";
		printVector(direction);
		ballZ = (-1.0f * direction);
		ballX = glm::normalize(cross(ballY, ballZ));		
		cout << " The acceleration is ";
		printVector(acceleration);
	}
	if (!(isPlayer))
	{
		//cout << " The other player's former final velocity is ";
		//printVector(finalVelocity);		
	}	
	/*Update the velociy */
	initVelocity = finalVelocity;


	/* The stopping condition */
	if (getMag(initVelocity) < 1.0 && getMag(acceleration) < 0.0001) {
		initVelocity = finalVelocity = acceleration = glm::vec3(0.0f);
	}
	else {
		finalVelocity = initVelocity + (acceleration * sec);
	}

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
	turn = 1; //reset to no turn


	if (isPlayer)
	{
		//cout << " The speed is " << getMag(finalVelocity) << endl;
		cout << " The finalVelocity  is ";
		printVector(finalVelocity);
	}
	else
	{		
		cout << " The other player's updated acceleration is ";
		printVector(acceleration);
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
	cout << " The diff is ";
	printVector(diff);
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
{	
	if (isPlayer)
		cout << " The player handling collision " << endl;
	else
		cout << " The other handling collision " << endl;	
	
	if (isPlayer)
	{
		cout << " The players initial pre collision velocity is ";
		printVector(initVelocity);
	}
	
	glm::vec3 collideVector = glm::normalize(otherObject->currPos - currPos);
	if (glm::dot(collideVector, initVelocity) > 0.0f)
	{
		
	}
	initVelocity = finalVelocity;
	float xSpeed = (((initVelocity.x  * (this->mass - otherObject->mass)) + (2.0f * otherObject->mass * otherObject->initVelocity.x)) / (this->mass + otherObject->mass));
	float zSpeed = (((initVelocity.z  * (this->mass - otherObject->mass)) + (2.0f * otherObject->mass * otherObject->initVelocity.z)) / (this->mass + otherObject->mass));

	finalVelocity = glm::vec3(xSpeed, 0.0f, zSpeed);
	matrixT->transformMatrix = glm::translate(glm::mat4(1.0f), finalVelocity * 600.0f) * matrixT->transformMatrix;

	
	//finalVelocity = glm::vec3(xSpeed, 0.0f, zSpeed) * 1.0f;
	acceleration = (finalVelocity - initVelocity)/100.0f;
	//acceleration = glm::vec3(0.0f);
	//acceleration = (finalVelocity - initVelocity) / ((float)clock() - t) + acceleration;
	t = clock();
	/*if (getMag(initVelocity) < getMag(otherObject->initVelocity))
	{
		finalVelocity * (10.0f);
		acceleration * (100.0f);
	} */
	/** Update the acceleration */
	if (isPlayer == false)
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
	}

}
glm::vec3 Ball::cross(glm::vec3 a, glm::vec3 b)
{
	float xVal = (a.y * b.z) - (a.z * b.y);
	float yVal = ((a.x * b.z) - (a.z * b.x)) * -1.0f;
	float zVal = (a.x * b.y) - (a.y * b.x);
	return glm::vec3(xVal, yVal, zVal);
}
