#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw();

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

bool running = true;

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


	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);
	draw_thread.detach();


	while (running)
	{
		glfwPollEvents();
	}

	draw_thread.join();

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

void draw()
{
	while (!glfwWindowShouldClose(*window))
	{
		populate_window(window);
		populate_window(secondWindow);
	}
	glfwWindowShouldClose(*secondWindow);
	running = false;
}

void framebuffer_size_callback(GLFWwindow *glfw_window, int w, int h)
{
	glfwMakeContextCurrent(glfw_window);
	glViewport(0,0,w,h);
}