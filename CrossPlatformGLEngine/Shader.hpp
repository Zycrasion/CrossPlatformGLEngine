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

	void SetUniformInt(const char* name, int value) { glUniform1i(this->GetLocation(name), value); }
	void SetUniformMatrix4(const char* name, glm::mat4 mat) { glUniformMatrix4fv(this->GetLocation(name), 1, GL_FALSE, glm::value_ptr(mat)); }

	operator GLuint() const { return this->program; }
};

Shader* ShaderFromFiles(const char* vPath, const char* fPath);