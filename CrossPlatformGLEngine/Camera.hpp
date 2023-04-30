#pragma once
#include "External.hpp"
#include "Window.hpp"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	float fov;
	float near;
	float far;

	Camera();

	glm::mat4 GetProjection(Window* win);
};