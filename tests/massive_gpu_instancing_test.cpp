#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>
#include <string>

using namespace std;
void draw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aOffset;\n"
"out vec3 vCol;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) + vec4(aOffset, 0.0, 0.0);\n"
"	vCol = (gl_Position.xyz + vec3(1.0,1.0,0.0))/2;\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"in vec3 vCol;"
"void main()\n"
"{\n"
"	FragColour = vec4(vCol, 1.0);\n"
"}\0";

const int instances = 10000000;

unsigned int shaderProgram;

float vertices[] = {
	-0.01f, -0.01f, 0.0f,
	 0.01f, -0.01f, 0.0f,
	 0.0f,  0.01f, 0.0f
};


Window* window;
int frames = 0;

double lastTime = 0;

int width, height;

Shader* shader;

Mesh* triangle;

unsigned int InstanceVBO;

unsigned int drawn_instances = instances;

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);
	win.SetResizable(false);

	window = &win;

	Mesh tri = Mesh(vertices, 3);
	triangle = &tri;

	shader = &Shader(vertexShaderSource, fragmentShaderSource);
	shader->use();

	int index = 0;
	float sq = floor(sqrtf(instances));
	float sq_hlf = sq / 2.f;
	float area = sq * 10.f;
	float *positions = new float[instances * 2];
	for (float x = 0; x < sq; x++)
	{
		for (float y = 0; y < sq; y++)
		{
			positions[index] = (x / sq_hlf) - 1.f;
			positions[index + 1] = (y / sq_hlf) - 1.f;
			index += 2;
		}
	}

	glBindVertexArray(triangle->VAO);

	glGenBuffers(1, &InstanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instances * 2, &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glVertexAttribDivisor(1, 1);  

	glClearColor(0.5, 0.25, 0.25, 1);

	glfwGetWindowSize(*window, &width, &height);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();

		// drawn_instances = (sin(glfwGetTime()) + 1.0) * instances/2;
		float x = sin(glfwGetTime());

		int index = 0;
		for (float x = 0; x < sq; x++)
		{
			for (float y = 0; y < sq; y++)
			{
				positions[index] += x;
				positions[index + 1] += 0;
				index += 2;
			}
		}
	}

	if (draw_thread.joinable())
	{
		draw_thread.join();
	}

	glfwTerminate();
	return 0;
}

void draw()
{
	while (!glfwWindowShouldClose(*window))
	{
		glfwMakeContextCurrent(*window);
		glViewport(0,0,width,height);

		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		glBindVertexArray(triangle->VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, drawn_instances);
		
		glfwSwapBuffers(*window);

		double currentTime = glfwGetTime();
		frames++;

		char title[256];

		title[255] = '\0';

		snprintf(
			title,
			255,
			"fps: %2.f triangles: %i",
			((float)frames) / (currentTime - lastTime),
			drawn_instances
		);

		window->SetTitle(title);

		if (currentTime - lastTime >= 1.0)
		{
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