#pragma once
#include "Utilities.hpp"
#include "External.hpp"

class Shader
{
public:
	unsigned int program;

	Shader(const char* vSrc, const char* fSrc);
	~Shader();
	void use();
	int GetLocation(const char* name) { return glGetUniformLocation(this->program, name); }

	operator GLuint() const { return this->program; }
};

Shader* ShaderFromFiles(const char* vPath, const char* fPath);