#ifndef _MT_H_
#define _MT_H_


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
#include <list>
#include "Group.h"

using namespace std;
class MatrixTransform : public Group
{
public:
	MatrixTransform();
	void draw(glm::mat4 cMatrix);
	void update();
	glm::mat4 transformMatrix;
};
#endif
