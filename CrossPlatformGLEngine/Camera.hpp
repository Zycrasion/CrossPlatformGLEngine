#pragma once
#include "External.hpp"
#include "Window.hpp"

class Camera
{
private:
	glm::mat4* overrideMatrix;
public:
	glm::vec3 position;
	glm::vec3 rotation;
	float fov;
	float near;
	float far;

	Camera();

	void BindOverrideTransform(glm::mat4*);
	glm::mat4 GetProjection(Window* win);
	glm::mat4 GetTransform();
};