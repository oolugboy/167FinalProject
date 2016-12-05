#ifndef _OBJECT_H_
#define _OBJECT_H_


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
#include <string>
#include "Node.h"

using namespace std;
class Object : public Node
{
public:
	Object();
	void draw(glm::mat4 cMatrix);
	void update();
	virtual bool collidesWith(Object * other) = 0;
	virtual void handleCollision(Object * other) = 0;
	glm::mat4 modelView;
	glm::mat4 toWorld;
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, NBO;
	GLuint uProjection, uModelview;
	glm::vec3 currPos;
	float scaleMult;
	float xBounds, yBounds, zBounds;
	float xWidth, yWidth, zWidth;
	float mass;	
	glm::vec3 finalVelocity;
	glm::vec3 acceleration;
	
};
#endif