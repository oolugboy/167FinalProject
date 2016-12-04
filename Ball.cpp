#include "Ball.h"
#include <cmath>

using namespace std;

clock_t Ball::t;
Ball::Ball(bool isPlayer, glm::vec3 position)
{	
	this->prevPos = this->currPos = this->initPos = position;
	this->isPlayer = isPlayer;		
	this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
	maxSpeed = 3.0f;

	/* The velocity values */
	initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);

	/** Bounds for the collision detection */
	xBounds = 0.0f, yBounds = 0.0f, zBounds = 0.0f;
	xWidth = 0.0f, yWidth = 0.0f, zWidth = 0.0f;
	toWorld = glm::mat4(1.0f);

	/* These are the objects relative x,y, and z coordinates */
	this->ballX = glm::vec3(1.0f, 0.0f, 0.0f);
	this->ballY = glm::vec3(0.0f, 1.0f, 0.0f);
	this->ballZ = glm::vec3(0.0f, 0.0f, 1.0f);	

	/* Set arbitatry value for the mass and acceleration */
	mass = 2.0;
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	inAir = false;
}
void Ball::draw(glm::mat4 cMatrix)
{
	toWorld = cMatrix;
	sphere->draw(cMatrix);
}
void Ball::update()
{
	clock_t temp = clock();
	currPos = initPos + glm::vec3(toWorld[3]);

	if (isPlayer)
	{
		/*cout << " The translations ";
		printVector(glm::vec3(toWorld[3]));
		cout << " The currPos set ";
		printVector(currPos);
		cout << " The prevPos set ";
		printVector(prevPos);
		cout << " The initial Pos set ";
		printVector(initPos);*/
	}
	//forwardForce = (-0.0008f * ballZ);
	checkMaxSpeed();
	updateVelocity((float)temp - t);
	t = temp;
}
//void Ball::turn(bool left, bool right, bool posAccel)
//{	
//	glm::vec3 turnForce = (0.002f * ballX);
//	if (left)
//	{
//		turnForce = (-1.0f * turnForce);
//	}
//	if (posAccel == false)
//	{		
//		forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);	
//		if (getMag(initVelocity) < 5.0f)
//		{
//			cout << " STOP !!" << endl;
//			acceleration = initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
//			return;
//		}
//	}	
//	glm::vec3 resForce = ((mass * acceleration) + (turnForce + forwardForce) * 0.7f);	
//	glm::vec3 frictionalForce = (0.0004f * (-1.0f * direction));
//	acceleration = (resForce + frictionalForce) / mass;
//}
void Ball::jump(bool accel)
{
	cout << " We are jumping " << endl;
	inAir = true;
	glm::vec3 upwardForce = (0.4f * ballY);
	if (accel)
		forwardForce = (-0.000300f * ballZ);
	else
		forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);
	/* Make sure the acceleration direction is updated */
	float a_mag = getMag(acceleration);
	acceleration = a_mag * direction;

	acceleration = ((mass * acceleration) + forwardForce + upwardForce) / mass;
}
void Ball::accelerate(bool posAccel)
{	
	glm::vec3 frictionalForce = (0.00029f * (-1.0f * direction));
	glm::vec3 gravitationalForce = glm::vec3(0.0f, 0.0f, 0.0f);



	//if(posAccel == false)
	//{
	//	forwardForce = glm::vec3(0.0f, 0.0f, 0.0f);
	//	if (getMag(initVelocity) < 5.0f)
	//	{
	//		cout << " STOP !!" << endl;
	//		acceleration = initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//		return;
	//	}
	//}

	if (posAccel) {
		forwardForce = (-0.000300f * ballZ);
	}
	else {
		forwardForce = glm::vec3(0.0f);
	}
	/* If the ball is in the air */
	if (inAir)
	{
		gravitationalForce = (-0.0029f * glm::vec3(0.0f, 1.0f, 0.0f));
		frictionalForce = forwardForce =  glm::vec3(0.0f, 0.0f, 0.0f);
		if (currPos.y < 0.01f)/* Offset from the ground */
		{
			/* Glide the ball back to the ground*/
			//forwardForce = 
		}
	}	
		
	//if (getMag(initVelocity) < 5.0f)
	//{
	//	cout << " STOP !!" << endl;
	//	acceleration = initVelocity = finalVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//}
	//else {
	
		/** Update the direction of the acceleration */
		float a_mag = getMag(acceleration);
		acceleration = a_mag * direction;

		acceleration = ((mass * acceleration) + forwardForce + frictionalForce + gravitationalForce) / mass;
		acceleration = (turn != 1) ? acceleration * 0.5f : acceleration * 1.0005f;
		cout << " The acceleration is ";
		printVector(acceleration);
	//}
}
/* This function updates the initial and the final velocity */
void Ball::updateVelocity(float sec)
{
	/* Update the object's directional attributes if the object is moving*/
	if (currPos != prevPos)
	{
		direction = glm::normalize(currPos - prevPos);

		cout << " The direction is ";
		printVector(direction);
		ballZ = (-1.0f * direction);
		ballX = glm::normalize(cross(ballY, ballZ));
		/*cout << " The moving ballX ";
		printVector(ballX);*/
		cout << " The acceleration is ";
		printVector(acceleration);
	}

	/*Update the velociy */
	initVelocity = finalVelocity;

	if (getMag(initVelocity) < 1.0 && getMag(acceleration) < 0.0001) {
		initVelocity = finalVelocity = acceleration = glm::vec3(0.0f);
	}
	else {
		finalVelocity = initVelocity + (acceleration * sec);
	}

	glm::vec3 turnForce = glm::vec3(0.0f);
	glm::vec3 turnVelocity = glm::vec3(0.0f);
	if (turn == 0) { // turn left
		turnForce = (-0.03000f * ballX);
		turnVelocity = (sec* turnForce / mass);
	}
	else if (turn == 2) { //turn right
		turnForce = (0.0300f * ballX) / mass;
		turnVelocity = (sec* turnForce / mass);
	}
	turn = 1; //reset to no turn


	finalVelocity = finalVelocity + turnVelocity;
	
	if (isPlayer)
	{
		cout << " The speed is " << getMag(finalVelocity) << endl;
		/*cout << " The direction is ";
		printVector(direction);*/
	}
	prevPos = currPos;
	updateBoxVals();
}
void Ball::updateBoxVals()
{
	glm::vec4 temp = toWorld[3];
	xBounds = (1.0f * toWorld[0][0]) + temp.x;
	yBounds = (1.0f * toWorld[1][1]) + temp.y;
	zBounds = (1.0f * toWorld[2][2]) + temp.z;
	xWidth = 2.0f * toWorld[0][0];
	yWidth = 2.0f * toWorld[1][1];
	zWidth = 2.0f * toWorld[2][2];
}

void Ball::checkMaxSpeed()
{
	if (getMag(finalVelocity) > maxSpeed)
	{
		finalVelocity = glm::normalize(finalVelocity) * 3.0f;
	}
}
float Ball::getMag(glm::vec3 dir)
{
	return sqrt(pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2));
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
	glm::vec3 actionForce = (acceleration * mass);
	glm::vec3 reactionForce = -1.0f * actionForce;
	// Incident force from the other object 
	glm::vec3 incidentForce = (otherObject->acceleration * otherObject->mass);
	acceleration = (actionForce + reactionForce + incidentForce) / mass;
	//For quick response
	acceleration = acceleration * (20.0f);
}
glm::vec3 Ball::cross(glm::vec3 a, glm::vec3 b)
{
	float xVal = (a.y * b.z) - (a.z * b.y);
	float yVal = ((a.x * b.z) - (a.z * b.x)) * -1.0f;
	float zVal = (a.x * b.y) - (a.y * b.x);
	return glm::vec3(xVal, yVal, zVal);
}
