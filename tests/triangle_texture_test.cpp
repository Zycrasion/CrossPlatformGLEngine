#define STB_IMAGE_IMPLEMENTATION
#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
		abort();
	}
}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

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
"in vec2 uv;\n"
"uniform sampler2D image;\n"
"uniform float m;\n"
"void main()\n"
"{\n"
"	FragColour = mix(texture(image,uv) , vec4(uv,0.0,1.0), m);\n"
"}\0";

unsigned int shaderProgram;

float vertices[] = {
//   VERTICES				 UV COORDINATES
	-0.5f, -0.5f, 0.0f,		 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		 1.0f, 1.0f,
	 0.0f,  0.5f, 0.0f,		 0.5f, 0.0f
};
unsigned int VBO;
unsigned int VAO;

Window* window;
Mesh* mesh;
Shader* shad;
int frames = 0;

double lastTime = 0;

int width, height;

int w, h, nrChannels;

Texture* c;
Texture* n;

int main()
{
	Window win = Initialise(720, 480, "hi");
	win.SetResizeCallback(framebuffer_size_callback);
	cout << glGetString(GL_VERSION) << endl << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	window = &win;

	Texture container = Texture("res/container.jpg");
	container.Bind(0);

	Texture nyan_cat = Texture("res/nyan_cat.png");

	c = &container;
	n = &nyan_cat;


	Shader shader(vertexShaderSource, fragmentShaderSource);
	shader.use();
	int loc = glGetUniformLocation(shader.program, "image");
	GL_CHECK(glUniform1i(loc, 0));

	shad = &shader;

	Mesh triangle = Mesh(vertices, 3, true);
	mesh = &triangle;

	glClearColor(0.2f, 0.1f, 0.1f, 1.0f);

	glfwGetWindowSize(*window, &width, &height);

	InitImgui(*window);

	glfwMakeContextCurrent(NULL);
	thread draw_thread(draw);
	draw_thread.detach();

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
	}

	if (draw_thread.joinable())
	{
		draw_thread.join();
	}


	glfwTerminate();
	return 0;
}

bool container = true;
float mix = 0.1f;

void draw()
{
	while (!glfwWindowShouldClose(*window))
	{
		glfwMakeContextCurrent(*window);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(glGetUniformLocation(*shad, "t"), sin((float)glfwGetTime()));

		glUniform1f(shad->GetLocation("m"), mix);

		shad->use();
		

		mesh->update(0.f);

		NewFrame();

		ImGui::Begin("Image Viewer");
		ImGui::Image((void*)(intptr_t)(container ? n : c)->GetTexture(), ImVec2(144, 144));
		if (ImGui::Button("Switch Texture"))
		{
			(container ? c : n)->Bind(0);
			container = !container;
		}
		ImGui::SliderFloat("Mix", &mix, -2.0f, 2.0f);
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
	DestroyImgui();

}

void framebuffer_size_callback(GLFWwindow* glfw_window, int w, int h)
{
	width = w;
	height = h;
}