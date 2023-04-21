#define STB_IMAGE_IMPLEMENTATION
#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aUV;\n"
"uniform float t;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
"	uv = aUV;\n"
"   gl_Position = vec4(aPos.x + t, aPos.yz, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"in vec2 uv;"
"void main()\n"
"{\n"
"	FragColour = vec4(uv, 0.0, 1.0);\n"
"}\0";

unsigned int shaderProgram;

float vertices[] = {
//   VERTICES				 UV COORDINATES
	-0.5f, -0.5f, 0.0f,		 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,		 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,		 0.5f, 1.0f
};
unsigned int VBO;
unsigned int VAO;

Window* window;
Mesh* mesh;
int frames = 0;

double lastTime = 0;

int width, height;

unsigned int texture;

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);

	window = &win;

	int w, h, nrChannels;
	unsigned char* data = stbi_load("res/container.jpg", &w, &h, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use(); 

	Mesh triangle = Mesh(vertices, 3, true);
	mesh = &triangle;

	glClearColor(0.2f, 0.1f, 0.1f, 1.0f);

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

		glUniform1f(0, sin((float)glfwGetTime()));
		
		mesh->update(0.f);

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