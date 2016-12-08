#ifndef _BALL_H_
#define _BALL_H_


#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <limits>
#include <time.h>
#include "Object.h"
#include "MatrixTransform.h"
#include <cmath>

using namespace std;
class Ball : public Object
{
public:
	Ball(bool isPlayer, glm::vec3 position, MatrixTransform * matrixT);
	~Ball();
	glm::vec3 initPos;
	glm::vec3 prevPos;	
	glm::vec3 direction;
	glm::vec3 cross(glm::vec3 a, glm::vec3 b);		
	void update();
	void draw(glm::mat4 cMatrix);
	void moveAgent(float sec);
	bool collidesWith(Object * otherObject);
	void handleCollision(Object * otherObject);
	bool isPlayer;
	void updateVelocity(float sec);
	void updateBoxVals();
	float getMag(glm::vec3 dir);
	clock_t t;
	glm::vec3 ballX;
	glm::vec3 ballY;
	glm::vec3 ballZ;
	void checkMaxSpeed();
	float maxSpeed;		
	void accelerate(bool posAccel);
	glm::vec3 forwardForce;
	void jump(bool accel);
	bool inAir;
	int turn;
	int collideAm;
};
#endif
