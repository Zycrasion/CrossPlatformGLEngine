#include <CrossPlatformGLEngine.hpp>
using namespace std;
void draw(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

Window* window;
int frames = 0;

double lastTime = 0;

int main()
{
	window = &Initialise(720, 480, "hi");
	window->SetResizeCallback(framebuffer_size_callback);

	glClearColor(0.5, 0.25, 0.25, 1);

	while (!glfwWindowShouldClose(*window))
	{
		draw(*window);
	}

	glfwTerminate();
	return 0;
}

void draw(GLFWwindow* glfw_window)
{
	glClear(GL_COLOR_BUFFER_BIT);

	double currentTime = glfwGetTime();
	frames++;

	if (currentTime - lastTime >= 1.0) // Check if last update was 1 second ago
	{
		char title[256];

		title[255] = '\0';

		snprintf(
			title,
			255,
			"hi %i",
			frames
		);

		window->SetTitle(title);

		frames = 0;
		lastTime = currentTime;
	}

	glfwSwapBuffers(*window);

	glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height)
{
	Window::DefaultResizeCallback(glfw_window, width, height);

	draw(glfw_window);
}