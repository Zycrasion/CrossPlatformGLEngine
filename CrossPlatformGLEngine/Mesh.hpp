#pragma once
#include "Component.hpp"
#include "External.hpp"

class Mesh : public Component
{
private:
	float* vertices;
	int length;
	unsigned int VBO;
	unsigned int VAO;

public:
	Mesh(float* vertices, int length);
	void init();
	void update(float deltaTime);
};