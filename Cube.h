#ifndef _CUBE_H_
#define _CUBE_H_

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


class Cube : public Geode
{
public:
	Cube(bool wired);
	Cube(GLchar * texture);
	~Cube();	
	void spin(float);
	void render();
	bool inBound() { return true; }

	GLuint vertNum = 36, texNum = 36;
	// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
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

	const glm::vec3 vertices[36] = {
		//Front
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		//Back
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, 1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),

		//Left
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  -1.0f),
		glm::vec3(-1.0f,  1.0f,  -1.0f),
		glm::vec3(-1.0f,  -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		//Right
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		//Bottom
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		//Top
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f)
	};
	glm::vec4 axisBounds[3];
	const glm::vec2 texture_v[36] = {
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
	};
};

#endif

