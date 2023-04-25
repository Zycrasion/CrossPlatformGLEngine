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

	bool use_indices;
	int indices_size;
	int indices_length;
	unsigned int* indices;

public:
	int length;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	Mesh(float* vertices, int length, unsigned int* indices = NULL, int indices_length = NULL, float* uv_coordinates = NULL, float* normals = NULL);
	void init();
	void update(float deltaTime);
	void Flip();

	void BindVBO(GLenum target = GL_ARRAY_BUFFER);
	void ReplaceVBOData(float* vertices);
};