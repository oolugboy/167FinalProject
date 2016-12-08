#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <cmath>

class Camera;

class Window
{
public:
	static int width;
	static int height;
	static bool debug;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow * window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow * window, double xOffset, double yOffset);
	static GLint shaderProgram2; // The seperate shader for the geometries
	static GLint agentShaderProgram;
	static glm::vec3 cam_pos;		// e  | Position of camera
	static glm::vec3 cam_look_at;	// d  | This is where the camera looks at
	static glm::vec3 cam_up;			// up | What orientation "up" is	
	static Camera * camera;
	static int isTexture;
};
#endif
