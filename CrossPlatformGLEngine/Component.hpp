#pragma once
class Node;

class Component
{
public:
	virtual void init() = 0;
	virtual void update(float deltaTime, Node* p_node) = 0;
};