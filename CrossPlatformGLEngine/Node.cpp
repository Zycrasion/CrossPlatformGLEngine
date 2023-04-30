#include "Node.hpp"

Node::Node()
{
	this->nodes = std::vector<Node*>();
	this->components = std::vector<Component*>();
	this->position = glm::vec3(0);
	this->scale = glm::vec3(1);
	this->rotation = glm::vec3(0);
}

void Node::init()
{
	for (int i = 0; i < this->nodes.size(); i++)
	{
		this->nodes[i]->init();
	}

	for (int i = 0; i < this->components.size(); i++)
	{
		this->components[i]->init();
	}
}

void Node::update(float deltaTime)
{
	for (int i = 0; i < this->nodes.size(); i++)
	{
		this->nodes[i]->update(deltaTime);
	}

	for (int i = 0; i < this->components.size(); i++)
	{
		this->components[i]->update(deltaTime);
	}
}

glm::mat4 Node::GetTransform()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, this->scale);
	model = glm::rotate(model, this->rotation.x, glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, this->rotation.z, glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, this->position);
	return model;
}
