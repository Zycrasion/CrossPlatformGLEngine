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
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glLinkProgram(this->program);

	CheckShaderProgramSuccess(this->program, true);
}

Shader::~Shader()
{
	glDeleteProgram(this->program);
}

void Shader::use()
{
	glUseProgram(this->program);
}

Shader* ShaderFromFiles(const char* vPath, const char* fPath)
{
	std::ifstream vertex_shader(vPath);
	std::string vertex_shader_src((std::istreambuf_iterator<char>(vertex_shader)),
		std::istreambuf_iterator<char>());

	std::ifstream fragment_shader(fPath);
	std::string fragment_shader_src((std::istreambuf_iterator<char>(fragment_shader)),
		std::istreambuf_iterator<char>());

	return new Shader(vertex_shader_src.c_str(), fragment_shader_src.c_str());
}