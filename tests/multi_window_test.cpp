#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw_first();
void draw_second();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const char *vertexShaderSource = "#version 460 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "uniform float t;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x + t, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
								   "out vec4 FragColour;\n"
								   "void main()\n"
								   "{\n"
								   "	FragColour = vec4(0.5, 0.5, 0.25, 1.0);\n"
								   "}\0";

unsigned int shaderProgram;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f};
unsigned int VBO;
unsigned int VAO;

Window *window;
Window *secondWindow;
int frames = 0;

double lastTime = 0;

int width, height;

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);

	window = &win;

	Window win2 = Initialise(480, 720, "hi2");
	win2.SetResizable(false);
	
	secondWindow = &win2;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glClearColor(0.5, 0.25, 0.25, 1);

	glfwGetWindowSize(*window, &width, &height);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw_first);
	draw_thread.detach();

	thread draw_thread2(draw_second);
	draw_thread2.detach();


	while ((!glfwWindowShouldClose(*window) && !glfwWindowShouldClose(*secondWindow)))
	{
		glfwMakeContextCurrent(win);
		glfwPollEvents();

		glfwMakeContextCurrent(win2);
		glfwPollEvents();
	}

	if (draw_thread.joinable())
	{
		draw_thread.join();
	}
	if (draw_thread2.joinable())
	{
		draw_thread2.join();
	}
		

	glfwTerminate();
	return 0;
}

void populate_window(Window *win)
{
	glfwMakeContextCurrent(*win);
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1f(0, sin((float)glfwGetTime()));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(*win);

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
			frames);

		win->SetTitle(title);

		frames = 0;
		lastTime = currentTime;
	}

}

void draw_first()
{
	while (!glfwWindowShouldClose(*window))
	{
		populate_window(window);
	}
}

void draw_second()
{
	while (!glfwWindowShouldClose(*secondWindow))
	{
		populate_window(secondWindow);
	}
}

void framebuffer_size_callback(GLFWwindow *glfw_window, int w, int h)
{
	width = w;
	height = h;
}