#ifndef _BUILDINGGRAMMAR_H
#define _BUILDINGGRAMMAR_H


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
#include "Cube.h"
#include "Sphere.h"
#include "MatrixTransform.h"
#include <string>
#include "time.h"

class BuildingGrammar
{
	public:
		BuildingGrammar();
		~BuildingGrammar();
		MatrixTransform * Build();

	private:
		bool ParseString();

};
#endif
