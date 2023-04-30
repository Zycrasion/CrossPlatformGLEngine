#include <CrossPlatformGLEngine.hpp>
#include <thread>
using namespace std;

int main();

void draw();

Window* window;

float scale = 0.5f;
double SavedX, SavedY; // For Mouse
bool Locked = false;

void Scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	scale += (float) yoffset / 100.0f;
}

void Key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == 'E' && action == GLFW_PRESS)
	{
		Locked = !Locked;
	}

	if (key == 'Q')
	{
		exit(1);
	}
}

int main()
{
	window = Initialise(720, 480, "Cube Test");
	glfwSetScrollCallback(*window, Scroll);
	glfwSetKeyCallback(*window, Key);

	glfwMakeContextCurrent(NULL);
	std::thread draw_thread(draw);
	draw_thread.detach();

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
		this_thread::sleep_for(16ms);
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

	Mesh* square = LoadObj("res/mario.obj");
	Mesh* light = LoadObj("res/light.obj");

	Shader* shader = ShaderFromFiles("res/Shaders/3D_Lit.vert", "res/Shaders/3D_Lit.frag");
	shader->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);
	Texture* MarioTexture = new Texture("res/mario.png");

	Texture* LightTexture = new Texture("res/Light.png");

	glClearColor(0.5, 0.2, 0.2, 1.0);


	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	int ProjectionLoc = shader->GetLocation("projection");
	int ModelLoc = shader->GetLocation("model");
	int ViewLoc = shader->GetLocation("view");
	int LightPositionLoc = shader->GetLocation("LightPosition");

	glm::vec3 LightPosition(1.0);

	glEnable(GL_DEPTH_TEST);


	while (!glfwWindowShouldClose(*window))
	{
		glfwMakeContextCurrent(*window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double x,y;
		if (!Locked)
			glfwGetCursorPos(*window, &SavedX, &SavedY);
		
		x = SavedX;
		y = SavedY;
		window->UpdateSize();

		glViewport(0,0, window->width, window->height);

		x /= 10.f;
		y /= 10.f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(scale / 5.f));
		model = glm::rotate(model, (float)glm::degrees(y / 500), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)-glm::degrees(x / 500), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, LightPosition);

		glm::vec3 pos = glm::vec3(model[3]);

		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)window->width / (float)window->height, 0.1f, 100.0f);
		glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(LightPositionLoc, 1, glm::value_ptr(pos));

		LightTexture->Bind(0);
		light->update(0.f);

		model = glm::scale(glm::mat4(1.0), glm::vec3(scale / 10.f));
		model = glm::rotate(model, (float)glm::degrees(y / 500), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)-glm::degrees(x / 500), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

		MarioTexture->Bind(0);
		square->update(0.f);


		NewFrame();

		try
		{
			ImGui::Begin("Window");
			ImGui::Checkbox("Lock Camera", &Locked);
			if (ImGui::Button("Reload Shaders"))
			{
				shader->~Shader();
				free(shader);

				shader = ShaderFromFiles("res/Shaders/3D_Lit.vert", "res/Shaders/3D_Lit.frag");
				shader->use();

				ProjectionLoc = shader->GetLocation("projection");
				ModelLoc = shader->GetLocation("model");
				ViewLoc = shader->GetLocation("view");
				LightPositionLoc = shader->GetLocation("LightPosition");
			}

			ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPosition), -10.f, 10.f);

			ImGui::End();
		}
		catch (...)
		{
			cout << "ERROR::IMGUI" << endl;
		}


		RenderImgui();

		glfwSwapBuffers(*window);


		this_thread::sleep_for(1.6ms);
	}

	DestroyImgui();
}