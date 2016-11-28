#ifndef _NODE_H_
#define _NODE_H_


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
#include "Window.h"
#include <vector>
#include <cmath>
#include <limits>
class Node
{
public:
	Node();
	virtual void draw(glm::mat4 cMatrix) = 0;
	virtual void update() = 0;	
	void printMatrix(glm::mat4 matrix);
	void printVector(glm::vec3 vector);
	bool culled;
	~Node();
};
#endif

