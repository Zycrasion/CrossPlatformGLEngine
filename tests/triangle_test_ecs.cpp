#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float t;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x + t, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"uniform vec3 colour;"
"void main()\n"
"{\n"
"	FragColour = vec4(colour, 1.0);\n"
"}\0";

unsigned int shaderProgram;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
unsigned int VBO;
unsigned int VAO;

Window* window;
int frames = 0;

double lastTime = 0;

int width, height;

Shader* shader;

Mesh* triangle;

int main()
{
	window = &Initialise(720, 480, "hi");
	window->SetResizeCallback(framebuffer_size_callback);

	triangle = &Mesh(vertices, 3);

	shader = &Shader(vertexShaderSource, fragmentShaderSource);
	shader->use();


	glClearColor(0.5, 0.25, 0.25, 1);


	glfwGetWindowSize(*window, &width, &height);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);
	draw_thread.detach();

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void draw()
{
	glfwMakeContextCurrent(*window);
	while (!glfwWindowShouldClose(*window))
	{
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		float t = sin((float)glfwGetTime());
		glUniform1f(glGetUniformLocation(shader->program, "t"), t);
		t = (t + 1.f) / 2.f;
		glUniform3f(glGetUniformLocation(shader->program, "colour"), t,t,t);
		triangle->update(0.f);

		glfwSwapBuffers(*window);

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
	}
}

void framebuffer_size_callback(GLFWwindow* glfw_window, int w, int h)
{
	width = w;
	height = h;
}