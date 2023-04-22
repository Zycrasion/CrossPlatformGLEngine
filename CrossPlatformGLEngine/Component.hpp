#pragma once

class Component
{
public:

	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
};