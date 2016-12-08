#include "Camera.h"

Camera::Camera()
{
	cam_pos = glm::vec3(0.0f, 5.0f, 15.0f);		// e  | Position of camera
	cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
	camera_mode = 0;
}


void Camera::changeMode() {
	this->camera_mode = (this->camera_mode + 1) % 3;
}


glm::mat4 Camera::getV()
{
	glm::mat4 V;

	glm::vec3 pos(0.0f, 80.0f, 0.0f);		// e  | Position of camera
	glm::vec3 look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
	glm::vec3 up(0.0f, 0.0f, 1.0f);			// up | What orientation "up" is 

	glm::vec3 player_pos, direction;
	switch (this->camera_mode) {
	case 1: //bird eye mode
		V = glm::lookAt(pos, look_at, up);
		break;

	case 2: //follow objects
		player_pos = glm::vec3(player->matrixT->transformMatrix[3]);
		direction = glm::normalize(player->direction);
		if (player->inAir) {
			pos = 12.0f * direction + player_pos;
		}
		else {
			pos = -12.0f * direction + player_pos;
			pos.y = 5.0f;
		}
		V = glm::lookAt(pos, player_pos, cam_up);
		break;


	default:
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		break;
	}
	return V;
}


