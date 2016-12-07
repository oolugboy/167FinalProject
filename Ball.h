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
#include "Sphere.h"
#include <time.h>
#include "Object.h"

using namespace std;
class Ball : public Object
{
public:
	Ball(bool isPlayer, glm::vec3 position);
	~Ball();
	glm::vec3 initPos;
	glm::vec3 prevPos;	
	glm::vec3 direction;
	glm::vec3 cross(glm::vec3 a, glm::vec3 b);	
	void draw(glm::mat4 cMatrix);
	void update();
	bool collidesWith(Object * otherObject);
	void handleCollision(Object * otherObject);
	bool isPlayer;
	void updateVelocity(float sec);
	void updateBoxVals();
	float getMag(glm::vec3 dir);
	Sphere * sphere;	
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
};
#endif
