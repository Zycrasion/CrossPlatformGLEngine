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
"uniform vec3 colour;\n"
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


float t = 0.f;
int t_loc = 0;
float colour[3] = { 0,0,0 };
int colour_loc = 1;

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);

	window = &win;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();
	t_loc = glGetUniformLocation(shader.program, "t");
	colour_loc = glGetUniformLocation(shader.program, "colour");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glClearColor(0.5, 0.25, 0.25, 1);

	glfwGetWindowSize(*window, &width, &height);

	InitImgui(*window);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);
	draw_thread.detach();

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
	}

	DestroyImgui();
	
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

		NewFrame();

		glUniform1f(t_loc, t);
		glUniform3f(colour_loc, colour[0], colour[1], colour[2]);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGui::Begin("Window");
		ImGui::SliderFloat("Triangle X Offset", &t, -1.f, 1.f);
		ImGui::ColorEdit3("Triangle Colour2", &colour[0]);
		ImGui::End();

		RenderImgui();

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