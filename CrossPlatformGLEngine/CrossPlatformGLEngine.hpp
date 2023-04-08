﻿// CrossPlatformGLEngine.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include "External.hpp"
#include "Window.hpp"

using namespace std;

Window Initialise(int width, int height, const char* title);

int CheckShaderSuccess(unsigned int shader, bool exit);
int CheckShaderProgramSuccess(unsigned int shaderProgram, bool exit);