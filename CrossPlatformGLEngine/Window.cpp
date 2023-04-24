#include "Window.hpp"
#include "../lib/glfw/src/internal.h"


Window::Window(int width, int height, const char* title)
{
	this->handle = glfwCreateWindow(width, height, title, NULL, NULL);
	this->unclosable = false;

	if (!this->handle)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(this->handle);
}

void Window::DefaultResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::SetResizeCallback(GLFWwindowsizefun fun)
{
	glfwSetWindowSizeCallback(this->handle, fun);
}

void Window::SetUnclosable()
{
	this->unclosable = true;
}

bool Window::GetShouldClose()
{
	if (this->unclosable)
	{
		return false;
	}
	return glfwWindowShouldClose(this->handle);
}

void Window::SetResizable(bool resizable)
{
	if (resizable)
	{
		glfwSetWindowSizeLimits(this->handle, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
	}
	else
	{
		int width, height;
		glfwGetWindowSize(this->handle, &width, &height);
		glfwSetWindowSizeLimits(this->handle, width, height, width, height);
	}
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(this->handle, title);
}

Window::~Window()
{
}