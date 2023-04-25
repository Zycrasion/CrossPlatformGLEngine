#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>
#include "../lib/glfw/src/internal.h"

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

float* rotated_verts;

unsigned int VBO;
unsigned int VAO;

Window* window;
int frames = 0;

double lastTime = 0;

int width, height;

unsigned int framebuffer;
unsigned int framebuffer_texture;

int main()
{
	Window win = Initialise(1080, 1080, "hi");
	win.SetResizable(false);

	window = &win;
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);
	void* VBO_PTR;
	VBO_PTR = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	

	if (!VBO_PTR)
	{
		return -1;
	}
	memcpy(VBO_PTR, vertices, sizeof(vertices));

	glUnmapBuffer(GL_ARRAY_BUFFER);

	rotated_verts = new float[sizeof(vertices) / sizeof(float)] {};
	memcpy(rotated_verts, vertices, sizeof(vertices));

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		
		//return -1;
	}
	glGenTextures(1, &framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

bool UpdateMeshEveryFrame = false;

void RenderImguiWindow()
{
	NewFrame();

	ImGui::Begin("Mesh Controller");
	ImGui::Image((void*)(intptr_t)framebuffer_texture, ImVec2(144, 144));
	if (ImGui::Button("Revert Mesh Data"))
	{
		memcpy(rotated_verts, vertices, sizeof(vertices));
	}
	if (ImGui::Button("Save Mesh Data"))
	{
		memcpy(vertices, rotated_verts, sizeof(vertices));
	}
	ImGui::Checkbox("Rotate Mesh", &UpdateMeshEveryFrame);
	if (UpdateMeshEveryFrame || ImGui::Button("Rotate Mesh Once"))
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		int length = sizeof(vertices) / sizeof(float);
		float t = glfwGetTime();
		for (int i = 0; i < length; i += 3)
		{
			float oldX = vertices[i + 0];
			float oldY = vertices[i + 1];
			rotated_verts[i + 0] = cos(t) * oldX - sin(t) * oldY;
			rotated_verts[i + 1] = sin(t) * oldX + cos(t) * oldY;
			rotated_verts[i + 2] = 0.f;
		}
		void* VBO_PTR = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(VBO_PTR, rotated_verts, length * sizeof(float));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	ImGui::End();
}

void RenderToFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, 800, 800);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniform1f(0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);;
	void* VBO_PTR;
	float* verts = new float[sizeof(vertices) / sizeof(float)] {};
	memcpy(verts, vertices, sizeof(vertices));
	for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 3)
	{
		// Flip Y Axis
		verts[i + 1] = -verts[i + 1];
	}

	VBO_PTR = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(VBO_PTR, verts, sizeof(vertices));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	VBO_PTR = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(VBO_PTR, rotated_verts, sizeof(vertices));
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void draw()
{
	glfwMakeContextCurrent(*window);
	InitImgui(*window);
	while (!glfwWindowShouldClose(*window))
	{
		RenderToFramebuffer();
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(0, sin((float)glfwGetTime()));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		RenderImguiWindow();

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
	DestroyImgui();
}

void framebuffer_size_callback(GLFWwindow* glfw_window, int w, int h)
{
	width = w;
	height = h;
}