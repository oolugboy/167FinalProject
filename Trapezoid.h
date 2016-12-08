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
	Trapezoid(GLchar *);
	~Trapezoid();	
	void spin(float);
	void render();
	bool inBound() { return true; }

	GLuint vertNum = 24, texNum = 24;

	glm::vec4 axisBounds[3];

	const glm::vec3 vertices[24] = {
		//Front
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-0.5f,  1.0f, 0.0f),
		glm::vec3(0.5f,  1.0f, 0.0f),
		glm::vec3(0.5f,  1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		//Back
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(0.5f, 1.0f,  0.0f),
		glm::vec3(-0.5f,  1.0f,  0.0f),
		glm::vec3(-0.5f,  1.0f,  0.0f),
		glm::vec3(-1.0f,  -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),

		//Left
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(0.5f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		//Right
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(0.5f, 1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),

		//Bottom
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

	};
	const glm::vec2 texture_v[24] = {
		glm::vec2(0.0f,0.0f), glm::vec2(0.25f,1.0f), glm::vec2(0.75f,1.0f), glm::vec2(0.75f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.25f,1.0f), glm::vec2(0.75f,1.0f), glm::vec2(0.75f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.5f,1.0f), glm::vec2(1.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.5f,1.0f), glm::vec2(1.0f,0.0f),
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,0.0f), glm::vec2(0.0f,0.0f),
	};
};

#endif

