#pragma once
#include "Component.hpp"
#include "External.hpp"

class Mesh : public Component
{
private:
	float* vertices;
	int size;

public:
	int length;
	unsigned int VBO;
	unsigned int VAO;

	Mesh(float* vertices, int length, bool uv_coordinates);
	void init();
	void update(float deltaTime);
	void Flip();

	void BindVBO(GLenum target = GL_ARRAY_BUFFER);
	void ReplaceVBOData(float* vertices);
};