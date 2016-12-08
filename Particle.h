#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
#include "Ball.h"
#include "Shader2.h"
#include <ctime>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Window.h"

class Window;

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(Shader shader, GLuint amount);
	// Update all particles
	void Update(GLfloat dt, Ball &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// Render all particles
	void Draw();
private:
	// State
	std::vector<Particle> particles;
	GLuint amount;
	// Render state
	Shader shader;
	GLuint texture;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle, Ball &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	GLuint loadTexture(GLchar * path);
};









//
//class Particle : public Geode
//{
//public:
//	Particle();
//	Particle(glm::vec3 pos, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotAngle, float scale);
//	~Particle();
//
//	void generateVAO();
//	void update();
//	bool isLive();
//	void render();
//	bool inBound() { return true; }
//
//	GLuint vertNum = 8, textNum = 8;
//	const glm::vec3 vertices[8] = {
//		// "Front" vertices
//		glm::vec3(-1.0f, -1.0f,  0.0f), glm::vec3(1.0f, -1.0f,  0.0f), glm::vec3(1.0f,  1.0f,  0.0f), glm::vec3(-1.0f,  1.0f,  0.0f)
//	};
//	const glm::vec2 texture_v[8] = {
//		// "Front" vertices
//		glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f,  1.0f), glm::vec2(0.0f,  1.0f)
//	};
//
//	// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
//	// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
//	const GLuint indices[6] = {
//		// Front face
//		0, 1, 2, 2, 3, 0
//	};	
//
//	glm::vec3 pos, o_pos;
//	glm::vec3 velocity;
//	float gravityEffect;
//	float lifeLength;
//	float rotAngle;
//	float scale;
//	clock_t time;
//
//	float elapsedTime = 0;
//
//	GLuint TBO;
//
//	Shader *particleShader;
//};

#endif

