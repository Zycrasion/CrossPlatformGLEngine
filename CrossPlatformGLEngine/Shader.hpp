#pragma once
#include "Utilities.hpp"
#include "External.hpp"

class Shader
{
public:
	unsigned int program;

	Shader(const char* vSrc, const char* fSrc);
	void use();
	int GetLocation(const char* name) { return glGetUniformLocation(this->program, name); }

	operator GLuint() const { return this->program; }
};