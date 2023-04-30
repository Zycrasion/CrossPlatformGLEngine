#include "Camera.hpp"

Camera::Camera()
{
	this->position = glm::vec3(0);
	this->rotation = glm::vec3(0);
	this->fov = 40.f;
	this->near = 0.1f;
	this->far = 100.f;
	this->overrideMatrix = nullptr;
}

void Camera::BindOverrideTransform(glm::mat4* mat)
{
	this->overrideMatrix = mat;
}

glm::mat4 Camera::GetProjection(Window* win)
{
	return glm::perspective(glm::radians(this->fov), (float) win->width / (float) win->height, 0.1f, 100.0f);
}

glm::mat4 Camera::GetTransform()
{
	if (this->overrideMatrix != nullptr)
	{
		return *this->overrideMatrix;
	}
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, this->rotation.x, glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, -this->rotation.y, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, this->rotation.z, glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, this->position);
	return model;
}