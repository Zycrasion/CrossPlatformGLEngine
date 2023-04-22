#pragma once
#include "External.hpp"

class Window
{
private:
	GLFWwindow* handle;

public:
	Window(int width, int height, const char* title);
	~Window();

	void SetResizable(bool resizable);
	void SetResizeCallback(GLFWwindowsizefun fun);

	void SetTitle(const char* title);

	static void DefaultResizeCallback(GLFWwindow* window, int width, int height);

	operator GLFWwindow* () const { return this->handle; }
};

