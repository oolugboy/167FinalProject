#ifndef _TRAPEZOID_H
#define _TRAPEZOID_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Geode.h"


class Trapezoid : public Geode
{
public:
	Trapezoid(bool wired);
	~Trapezoid();	
	void spin(float);
	void render();
	bool inBound() { return true; }

	GLuint vertNum = 8, normNum = 8;
	// Define the coordinates and indices needed to draw the Trapezoid. Note that it is not necessary
	// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
	// This just looks nicer since it's easy to tell what coordinates/indices belong where.
	const glm::vec3 faceNormals[6] = {
		//Front face
		glm::vec3(0, 0, 1),
		//Top face
		glm::vec3(0, 1, 0),
		//Backface
		glm::vec3(0, 0,-1),
		//Bottom face
		glm::vec3(0, -1, 0),
		//Left face
		glm::vec3(-1, 0, 0),
		//Right face
		glm::vec3( 1, 0, 0 )
	};
	enum {
		frontFaceNorm,
		topFaceNorm,
		backFaceNorm,
		bottomFaceNorm,
		leftFaceNorm,
		rightFaceNorm
	};
	const glm::vec3 vertices[6] = {
		// "Bottom" vertices
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f,  1.0f),  glm::vec3(-1.0f, -1.0f, 1.0f),
		// "Top" vertices
		glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f)
	};
	const glm::vec3 normals[8] = {
		// The front vertices
		glm::normalize((faceNormals[bottomFaceNorm] + faceNormals[leftFaceNorm] + faceNormals[frontFaceNorm])),
		glm::normalize((faceNormals[bottomFaceNorm] + faceNormals[rightFaceNorm] + faceNormals[frontFaceNorm])),
		glm::normalize((faceNormals[topFaceNorm] + faceNormals[rightFaceNorm] + faceNormals[frontFaceNorm])),
		glm::normalize((faceNormals[topFaceNorm] + faceNormals[leftFaceNorm] + faceNormals[frontFaceNorm])),

		// The back vertices
		glm::normalize((faceNormals[bottomFaceNorm] + faceNormals[leftFaceNorm] + faceNormals[backFaceNorm])),
		glm::normalize((faceNormals[bottomFaceNorm] + faceNormals[rightFaceNorm] + faceNormals[backFaceNorm])),
		glm::normalize((faceNormals[topFaceNorm] + faceNormals[rightFaceNorm] + faceNormals[backFaceNorm])),
		glm::normalize((faceNormals[topFaceNorm] + faceNormals[leftFaceNorm] + faceNormals[backFaceNorm]))
	};
	glm::vec4 axisBounds[3];

	// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
	// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
	const GLuint indices[8][3] = {
		// Front face 1
		{0, 4, 5},
		// Front face 2
		{5, 1, 0},
		// Right face
		{5, 2, 1},
		// Back face 1
		{2, 5, 4},
		// Back face 2
		{4, 3, 2},
		// Left face
		{4, 0, 3},
		// Bottom face 1
		{0, 1, 2},
		// Bottom face 2
		{2, 3, 0},
	};	
};

#endif

