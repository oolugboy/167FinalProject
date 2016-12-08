#ifndef _CAMERA_H_
#define _CAMERA_H_


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

class Camera
{
public:
	Camera();

	void changeMode();
	glm::mat4 getV();

	glm::vec3 cam_pos;
	glm::vec3 cam_look_at;
	glm::vec3 cam_up;

	Ball* player;
	int camera_mode;
};
#endif
