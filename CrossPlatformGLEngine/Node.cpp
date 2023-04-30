#include "Node.hpp"

Node::Node()
{
	this->nodes = std::vector<Node*>();
	this->components = std::vector<Component*>();
	this->position = glm::vec3(0);
	this->position = glm::vec3(0);
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
	glm::mat4 model = glm::mat4();
	return glm::mat4();
}
