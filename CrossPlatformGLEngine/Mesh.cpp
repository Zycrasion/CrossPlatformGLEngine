#include "Mesh.hpp"

Mesh::Mesh(float* vertices, int length, unsigned int* indices, int indices_length, float* uv_coordinates, float* normals)
{
	this->vertices = vertices;
	this->length = length;
	this->vertices_size = this->length * sizeof(float) * 3;
	this->uv_size = this->length * sizeof(float) * (uv_coordinates != NULL ? 2 : 0);
	this->normals_size = this->vertices_size * (normals != NULL ? 1 : 0);
	this->uv_coordinates = uv_coordinates;
	this->use_indices = indices != NULL;

	if (this->use_indices)
	{
		this->indices = indices;
		this->indices_length = indices_length;
		this->indices_size = this->indices_length * sizeof(int);
	}

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Make Vertex buffer
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices_size + this->uv_size + this->normals_size, NULL, GL_STATIC_DRAW);

	if (this->use_indices)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices_size, this->indices, GL_STATIC_DRAW);
	}

	int offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, this->vertices_size, vertices);
	offset += this->vertices_size;

	if (uv_coordinates != NULL)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, this->uv_size, uv_coordinates);
		offset += this->uv_size;
		if (normals != NULL)
		{
			glBufferSubData(GL_ARRAY_BUFFER, offset, this->normals_size, normals);
			offset += this->normals_size;
		}
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (uv_coordinates != NULL)
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(this->vertices_size));
		glEnableVertexAttribArray(1);
		if (normals != NULL)
		{
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(this->vertices_size + this->uv_size));
			glEnableVertexAttribArray(2);
		}
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
	if (this->use_indices)
	{
		glDrawElements(GL_TRIANGLES, this->indices_length, GL_UNSIGNED_INT, 0);
	} else 
	{
		glDrawArrays(GL_TRIANGLES, 0, this->length);
	}
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
	memcpy(this->vertices, vertices, this->vertices_size);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices_size, this->vertices);
}
