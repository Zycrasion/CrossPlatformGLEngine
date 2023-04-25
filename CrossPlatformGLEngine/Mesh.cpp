#include "Mesh.hpp"

Mesh::Mesh(float* vertices, int length, bool uv_coordinates)
{
	this->vertices = vertices;
	this->length = length;
	this->size = this->length * sizeof(float) * 3;
	this->uv_coords = uv_coordinates;

	if (uv_coordinates)
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// Make Vertex buffer
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->length * 5, this->vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// Make Vertex buffer
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->length * 3, this->vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
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

void Mesh::Flip()
{
	for (int i = 0; i < this->length * (this->uv_coords ? 5 : 3); i += (this->uv_coords ? 5 : 3))
	{
		// Flip Y Axis
		this->vertices[i + 1] = -this->vertices[i + 1];
	}
	this->ReplaceVBOData(this->vertices);
}

void Mesh::BindVBO(GLenum target)
{
	glBindBuffer(target, this->VBO);
}

void Mesh::ReplaceVBOData(float* vertices)
{
	this->BindVBO();
	this->vertices = new float[this->length * 3];
	memcpy(this->vertices, vertices, this->size);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->size, this->vertices);
}
