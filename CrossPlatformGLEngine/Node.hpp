#pragma once
#include "External.hpp"
#include "Component.hpp"

class Node
{
public:
	std::vector<Node> nodes;
	std::vector<Component> components;

	Node();
	
	void init();
	void update(float deltaTime);
};

