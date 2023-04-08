#include <CrossPlatformGLEngine.hpp>
using namespace std;
void draw(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

unsigned int vertexShader;
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

unsigned int fragmentShader;
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"void main()\n"
"{\n"
"	FragColour = vec4(0.5, 0.5, 0.25, 1.0);\n"
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

int main()
{
	window = &Initialise(720, 480, "hi");
	window->SetResizeCallback(framebuffer_size_callback);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	CheckShaderSuccess(vertexShader, true);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	CheckShaderSuccess(fragmentShader, true);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	CheckShaderProgramSuccess(shaderProgram, true);

	glUseProgram(shaderProgram);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(*window);

	glfwPollEvents();

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

void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height)
{
	Window::DefaultResizeCallback(glfw_window, width, height);

	draw(glfw_window);
}