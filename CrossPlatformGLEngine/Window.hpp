#pragma once
#include "External.hpp"

class Window
{
private:
	bool unclosable;
	GLFWwindow* handle;
public:
	int width, height;
	Window(int width, int height, const char* title);
	~Window();

	void SetResizable(bool resizable);
	void SetResizeCallback(GLFWwindowsizefun fun);
	void SetUnclosable(bool unclosable);
	bool GetShouldClose();
	void SetTitle(const char* title);
	void UpdateSize() { glfwGetWindowSize(*this, &this->width, &this->height); }

	static void DefaultResizeCallback(GLFWwindow* window, int width, int height);

	operator GLFWwindow* () const { return this->handle; }
};

