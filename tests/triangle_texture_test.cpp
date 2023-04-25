#include <CrossPlatformGLEngine.hpp>
#include <thread>
#include <cmath>

using namespace std;
void draw();

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
"uniform float flip;"
"void main()\n"
"{\n"
"	uv = aUV;\n"
"   gl_Position = vec4(aPos.x + t, aPos.y * flip, aPos.z, 1.0);\n"
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

float vertices[] = {
//   VERTICES				 UV COORDINATES
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float uv_coordinates[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.5f, 0.0f
};

Window* window;
Mesh* mesh;
Shader* shader;
int frames = 0;
double lastTime = 0;

Texture* container;
Texture* nyan_cat;

Framebuffer* render_tex;

std::vector<Texture*> textures;

int index = 0;
float mix = 0.1f;

int main()
{
	window = Initialise(720, 480, "hi");

	container = new Texture("res/container.jpg");
	nyan_cat = new Texture("res/nyan_cat.png");
	render_tex = new Framebuffer(800, 800);

	textures = std::vector<Texture*>();
	textures.push_back(container);
	textures.push_back(nyan_cat);
	textures.push_back(render_tex->GetRenderTexture());
	textures.push_back(new Texture("res/burrito.png"));

	textures[index]->Bind(0);

	shader = new Shader(vertexShaderSource, fragmentShaderSource);
	shader->use();

	int loc = glGetUniformLocation(shader->program, "image");
	GL_CHECK(glUniform1i(loc, 0));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mesh triangle = Mesh(vertices, 3, uv_coordinates);
	mesh = &triangle;

	glClearColor(0.2f, 0.1f, 0.1f, 1.0f);

	while (1)
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
		{
			break;
		}
		printf("OpenGL error %08x\n", error);
	}

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

void RenderTexture()
{
	render_tex->Bind();
	glUniform1f(shader->GetLocation("t"), 0.f);
	glUniform1f(shader->GetLocation("m"), sin(glfwGetTime()));
	glUniform1f(shader->GetLocation("flip"), -1.f);
	textures[0]->Bind(0);
	mesh->update(0.f);
	render_tex->Unbind();
	textures[index]->Bind(0);
	glUniform1f(shader->GetLocation("flip"), 1.f);
}

void draw()
{
	while (!glfwWindowShouldClose(*window))
	{
		RenderTexture();

		glfwMakeContextCurrent(*window);
		window->UpdateSize();
		glViewport(0, 0, window->width, window->height);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(glGetUniformLocation(*shader, "t"), sin((float)glfwGetTime()));

		glUniform1f(shader->GetLocation("m"), mix);

		shader->use();

		mesh->update(0.f);

		NewFrame();

		ImGui::Begin("Image Viewer");
		ImGui::Image((void*)(intptr_t)textures[index]->GetTexture(), ImVec2(144, 144));
		if (ImGui::Button("Switch Texture"))
		{
			index++;
			if (index >= textures.size())
				index = 0;
			
			textures[index]->Bind(0);
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