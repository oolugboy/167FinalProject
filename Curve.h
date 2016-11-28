#ifndef _CURVE_H_
#define _CURVE_H_

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
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <vector>

class Curve
{
public:
	Curve(std::vector<glm::vec3>* cp, int interval);
	~Curve();

	GLuint generateVAO();


	glm::mat4 toWorld;

	void draw(Shader shader);
	void update();

	void computePoints();

	glm::vec3 getPosition(float t);
	glm::vec3 getPosition(int index);


	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	//std::vector<GLfloat> vertices;
	std::vector<glm::vec3> vertices;
	std::vector<int> indices;

	std::vector<glm::vec3>* control_points;
	int interval;
	float step;
};

#endif