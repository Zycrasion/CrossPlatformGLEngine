#include <CrossPlatformGLEngine.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>

void draw();

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aUV;\n"
"layout (location = 2) in vec3 aNormal;\n"
"out vec2 uv;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	uv = aUV;\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColour;\n"
"in vec2 uv;\n"
"uniform sampler2D image;\n"
"void main()\n"
"{\n"
"	FragColour = texture(image,uv);\n"
"}\0";

void PrintOGLError()
{
	GLenum error = glGetError();
	if (error == GLFW_NO_ERROR)
		return;

	printf("OpenGL error: %08x", error);
}



float vertices[] = {
	-1.0, -1.0, 0.0f,
	-1.0,  1.0,	0.0f,
	 1.0,  1.0,	0.0f,
	 1.0, -1.0,	0.0f
};

float uv_coords[] = {
	 0.0,  0.0,
	 0.0,  1.0,
	 1.0,  1.0,
	 1.0,  0.0
};

unsigned int indices[] = {
	0,1,2,
	2,0,3,
};

Window* window;

int main()
{
	window = Initialise(720, 480, "Cube Test");
	window->SetResizable(false);

	glfwMakeContextCurrent(NULL);
	std::thread draw_thread(draw);
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
}

void draw()
{
	glfwMakeContextCurrent(*window);
	InitImgui(*window);

	Mesh* square = new Mesh(vertices, 4, indices, 6, uv_coords);
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);
	shader->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);
	Texture* texture = new Texture("res/burrito.png");
	texture->Bind(0);

	glUniform1i(shader->GetLocation("image"), 0);


	glClearColor(0.5, 0.2, 0.2, 1.0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	int ProjectionLoc = shader->GetLocation("projection");
	int ModelLoc = shader->GetLocation("model");
	int ViewLoc = shader->GetLocation("view");

	while (!glfwWindowShouldClose(*window))
	{
		glfwMakeContextCurrent(*window);
		glClear(GL_COLOR_BUFFER_BIT);
		glm::mat4 transform = glm::mat4(1.0f);

		glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(view));

		square->update(0.f);
		glfwSwapBuffers(*window);
	}

	DestroyImgui();
}