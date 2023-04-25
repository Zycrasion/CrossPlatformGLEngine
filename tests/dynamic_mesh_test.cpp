#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>
using namespace std;
void draw();

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

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float* saved_verts;
float* rotated_verts;
Mesh* mesh;
Window* window;
Framebuffer* framebuffer;
bool UpdateMeshEveryFrame = false;

int main()
{
	window = Initialise(800, 800, "hi");
	mesh = new Mesh(vertices, 3, false);

	rotated_verts = new float[sizeof(vertices) / sizeof(float)] {};
	memcpy(rotated_verts, vertices, sizeof(vertices));

	saved_verts = new float[sizeof(vertices) / sizeof(float)] {};
	memcpy(saved_verts, vertices, sizeof(vertices));

	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();

	framebuffer = new Framebuffer(800, 800);
	
	glClearColor(0.5, 0.25, 0.25, 1);

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

void RenderImguiWindow()
{
	NewFrame();
	ImGui::Begin("Mesh Controller");
	ImGui::Image((void*)framebuffer->GetTexture(), ImVec2(144, 144 * ((float)window->height / (float)window->width)));
	if (ImGui::Button("Revert Mesh Data"))
		memcpy(rotated_verts, vertices, sizeof(vertices));

	if (ImGui::Button("Save Mesh Data"))
		memcpy(vertices, rotated_verts, sizeof(vertices));

	ImGui::Checkbox("Rotate Mesh", &UpdateMeshEveryFrame);
	if (UpdateMeshEveryFrame || ImGui::Button("Rotate Mesh Once"))
	{
		float t = glfwGetTime();
		for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 3)
		{
			rotated_verts[i + 0] = cos(t) * saved_verts[i + 0] - sin(t) * saved_verts[i + 1];
			rotated_verts[i + 1] = sin(t) * saved_verts[i + 0] + cos(t) * saved_verts[i + 1];
		}
	}
	ImGui::End();
}

void RenderToFramebuffer()
{
	framebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1f(0, 0);
	mesh->ReplaceVBOData(vertices);
	mesh->Flip();
	mesh->update(0.f);
	mesh->ReplaceVBOData(rotated_verts);

	framebuffer->Unbind();
}

void draw()
{
	glfwMakeContextCurrent(*window);
	InitImgui(*window);
	while (!glfwWindowShouldClose(*window))
	{
		RenderToFramebuffer();
		window->UpdateSize();
		glViewport(0, 0, window->width, window->height);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(0, sin((float)glfwGetTime()));
		mesh->update(0.f);
		
		RenderImguiWindow();
		RenderImgui();
		glfwSwapBuffers(*window);
	}
	DestroyImgui();
}