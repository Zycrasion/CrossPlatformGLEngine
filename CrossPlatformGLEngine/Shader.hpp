#pragma once
#include "Utilities.hpp"

class Shader
{
public:
	unsigned int program;

	Shader(const char* vSrc, const char* fSrc);
	void use();
};