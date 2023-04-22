#include "Mesh.hpp"

Mesh::Mesh(float* vertices, int length)
{
	this->vertices = vertices;
	this->length = length;

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Make Vertex buffer
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->length * 3, this->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Mesh::init()
{
	// Do nothing since the Mesh can only be initialised once
}

void Mesh::update(float deltaTime)
{
	// Create
	glBindVertexArray(this->VAO);

	glDrawArrays(GL_TRIANGLES, 0, this->length);
}