#pragma once
#include "Utilities.hpp"
#include "External.hpp"

class Shader
{
public:
	unsigned int program;

	Shader(const char* vSrc, const char* fSrc);
	void use();

	operator GLuint() const { return this->program; }
};