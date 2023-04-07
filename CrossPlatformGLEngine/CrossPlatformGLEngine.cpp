// CrossPlatformGLEngine.cpp : Defines the entry point for the application.
//

#include "CrossPlatformGLEngine.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(720, 480, "GLFW window", NULL, NULL);

	if (!window)
	{
		cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialise glad";
		glfwTerminate();
		return -1;
	}

	glClearColor(0.5, 0.25, 0.25, 1);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
