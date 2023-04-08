#include "CrossPlatformGLEngine.hpp"

Window Initialise(int width, int height, const char* title)
{
	if (!glfwInit())
		exit(-1);

	Window window = Window(width, height, title);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialise glad";
		glfwTerminate();
		exit(-1);
	}

	return window;
}