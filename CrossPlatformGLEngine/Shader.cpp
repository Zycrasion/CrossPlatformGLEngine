#include "Shader.hpp"

Shader::Shader(const char* vSrc, const char* fSrc)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSrc, NULL);
	glCompileShader(vertexShader);

	CheckShaderSuccess(vertexShader, true);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSrc, NULL);
	glCompileShader(fragmentShader);

	CheckShaderSuccess(fragmentShader, true);

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);

	CheckShaderProgramSuccess(this->program, true);
}

void Shader::use()
{
	glUseProgram(this->program);
}