#ifndef _BUILDING_H_
#define _BUILDING_H_


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
#include "Sphere.h"
#include <SOIL.h>
#include <cmath>

class Building : public Object
{
public:
	Building(bool isPlayer, glm::vec3 position, MatrixTransform * matrixT);
	glm::vec3 initPos;
	glm::vec3 prevPos;	
	glm::vec3 direction;
	void update();
	void draw(glm::mat4 cMatrix);
	void moveAgent(float sec);
	bool collidesWith(Object * otherObject);
	void handleCollision(Object * otherObject);
	bool isPlayer;
	void updateBoxVals();
	clock_t t;
	glm::vec3 BuildingX;
	glm::vec3 BuildingY;
	glm::vec3 BuildingZ;
	float maxSpeed;		
	void accelerate(bool posAccel);
	glm::vec3 forwardForce;
	static vector <const GLchar *> playSkyFaces;
	static GLuint playerTexID;
	static GLuint aITexID;
	int collideAm;
	bool outOfBounds();
};
#endif
