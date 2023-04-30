#include "Camera.hpp"

Camera::Camera()
{
	this->position = glm::vec3(0);
	this->rotation = glm::vec3(0);
	this->fov = 40.f;
	this->near = 0.1f;
	this->far = 100.f;
}

glm::mat4 Camera::GetProjection(Window* win)
{
	return glm::perspective(glm::radians(this->fov), (float) win->width / (float) win->height, 0.1f, 100.0f);
}

