#ifndef _SPHERE_H_
#define _SPHERE_H_


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
#include "Geode.h"

using namespace std;
class Sphere : public Geode
{
public:
	Sphere(float radius, bool wired);
	void render();
	int latNum;
	int longNum;
	float m_radius;
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > normals;
	vector< int > indices;
	void genVertices();
	void genIndices();
	const float PI = 3.1415927;	
	bool inBound();
	vector <float > dVals;
	vector < glm::vec3 > frustNorms;	
};
#endif
