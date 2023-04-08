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

int CheckShaderSuccess(unsigned int shader, bool exit_on_fail = true)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER Compile failed InfoLog:\n" << infoLog << "\n";

		return -1;
		if (exit_on_fail)
		{
			exit(-1);
		}
	}
	return 1;
}

int CheckShaderProgramSuccess(unsigned int shader, bool exit_on_fail = true)
{
	int success;
	char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER Compile failed InfoLog:\n" << infoLog << "\n";

		return -1;
		if (exit_on_fail)
		{
			exit(-1);
		}
	}
	return 1;
}