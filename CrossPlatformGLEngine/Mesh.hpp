#pragma once
#include "Component.hpp"
#include "External.hpp"

class Mesh : public Component
{
private:
	int vertices_size;
	float* vertices;
	int uv_size;
	float* uv_coordinates;
	int normals_size;
	float* normals;
	bool uv_coords;

public:
	int length;
	unsigned int VBO;
	unsigned int VAO;

	Mesh(float* vertices, int length, float* uv_coordinates = NULL, float* normals = NULL);
	void init();
	void update(float deltaTime);
	void Flip();

	void BindVBO(GLenum target = GL_ARRAY_BUFFER);
	void ReplaceVBOData(float* vertices);
};