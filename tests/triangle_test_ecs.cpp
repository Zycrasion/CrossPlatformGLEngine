#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>
#include <string>

using namespace std;
void draw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec3 vCol;\n"
"uniform vec2 offsets[5000];\n"
"void main()\n"
"{\n"
"	vec2 offset = offsets[gl_InstanceID];\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) + vec4(offset, 0.0, 0.0);\n"
"	vCol = gl_Position.xyz;\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"in vec3 vCol;"
"void main()\n"
"{\n"
"	FragColour = vec4(vCol, 1.0);\n"
"}\0";

const int instances = 5000;

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

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);

	window = &win;

	Mesh m = Mesh(vertices, 3, false);
	triangle = &m;

	shader = &Shader(vertexShaderSource, fragmentShaderSource);
	shader->use();

	int index = 0;
	float sq = floor(sqrtf(instances));
	float sq_hlf = sq / 2.f;
	float positions[2];
	for (float x = 0; x < sq; x++)
	{
		for (float y = 0; y < sq; y++)
		{
			positions[0] = (x + 0.5f - sq_hlf) / sq_hlf;
			positions[1] = (y + 0.5f - sq_hlf) / sq_hlf;
			int loc = glGetUniformLocation((GLuint)*shader, ("offsets[" + std::to_string(index) + "]").c_str());

			glUniform2fv(loc, 2, positions);
			index++;
		}
	}

	for (int i = 0; i < 100; i++)
	{

	}

	glClearColor(0.5, 0.25, 0.25, 1);

	glfwGetWindowSize(*window, &width, &height);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
	}

	draw_thread.join();

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
		int numOfInstances = instances;
		float time = (float)glfwGetTime();

		int TLoc = glGetUniformLocation(shader->program, "t");
		int JLoc = glGetUniformLocation(shader->program, "j");
		int ColLoc = glGetUniformLocation(shader->program, "colour");

		glBindVertexArray(triangle->VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, numOfInstances);

		shader->use();
		

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
				"fps: %i triangles: %i",
				frames,
				numOfInstances
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