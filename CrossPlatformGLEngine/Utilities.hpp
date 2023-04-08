#pragma once
#include "Window.hpp"

Window Initialise(int width, int height, const char* title);

int CheckShaderSuccess(unsigned int shader, bool exit);
int CheckShaderProgramSuccess(unsigned int shaderProgram, bool exit);