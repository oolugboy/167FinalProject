#include "Camera.h"

Camera::Camera(Ball * p)
{
	this->player = p;
	cam_pos = glm::vec3(0.0f, 5.0f, 15.0f);		// e  | Position of camera
	cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
	camera_mode = 0;
}


void Camera::changeMode() {
	camera_mode = camera_mode % 3;
}


glm::mat4 Camera::getV()
{
	glm::mat4 V;

	glm::vec3 pos(0.0f, 50.0f, 0.0f);		// e  | Position of camera
	glm::vec3 look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
	glm::vec3 up(0.0f, 0.0f, 1.0f);			// up | What orientation "up" is 

	switch (camera_mode) {
	case 1: //bird eye mode
		V = glm::lookAt(pos, look_at, up);
		break;

	default:
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		break;
	}
	return glm::mat4();
}


