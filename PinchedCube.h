#ifndef _PINCHEDCUBE_H
#define _PINCHEDCUBE_H

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


class PinchedCube : public Geode
{
public:
	PinchedCube(bool wired);
	~PinchedCube();	
	void spin(float);
	void render();
	bool inBound() { return true; }

	GLuint vertNum = 8, normNum = 8;
	// Define the coordinates and indices needed to draw the PinchedCube. Note that it is not necessary
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
	const glm::vec3 vertices[8] = {
		// "Front" vertices
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.5f,  1.0f,  0.5f), glm::vec3(-0.5f,  1.0f,  0.5f),
		// "Back" vertices
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.5f,  1.0f, -0.5f), glm::vec3(-0.5f,  1.0f, -0.5f)
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
	const GLuint indices[6][6] = {
		// Front face
		{0, 1, 2, 2, 3, 0},
		// Top face
		{1, 5, 6, 6, 2, 1},
		// Back face
		{7, 6, 5, 5, 4, 7},
		// Bottom face
		{4, 0, 3, 3, 7, 4},
		// Left face
		{4, 5, 1, 1, 0, 4},
		// Right face
		{3, 2, 6, 6, 7, 3}
	};	
};

#endif

