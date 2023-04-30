#pragma once
#include "External.hpp"
#include "Component.hpp"

class Node
{
public:
	std::vector<Node> nodes;
	std::vector<Component> components;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Node();
	
	void init();
	void update(float deltaTime);
	glm::mat4 GetTransform();
};

