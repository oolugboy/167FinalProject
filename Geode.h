#ifndef _GEODE_H_
#define _GEODE_H_


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
class Geode : public Node
{
public:
	Geode();
	void draw(glm::mat4 cMatrix);
	void update();
	virtual void render() = 0;
	virtual bool inBound() = 0;	
	glm::mat4 modelView;
	glm::mat4 toWorld;
	bool wired;
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, NBO;
	GLuint uProjection, uModelview;
	glm::vec3 currPos;
	float scaleMult;
	float xBounds, yBounds, zBounds;
	float xWidth, yWidth, zWidth;

};
#endif
