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

bool focused = true;

void Focus(GLFWwindow* window, int glfw_focused)
{
	focused = glfw_focused == GLFW_TRUE;
}

int main()
{
	window = Initialise(720, 480, "Cube Test");
	glfwSetWindowFocusCallback(*window, Focus);
	glfwSetScrollCallback(*window, Scroll);
	glfwSetKeyCallback(*window, Key);

	glfwMakeContextCurrent(NULL);
	std::thread draw_thread(draw);
	draw_thread.detach();

	while (!glfwWindowShouldClose(*window))
	{
		glfwPollEvents();
		this_thread::sleep_for(1.6ms);
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
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5, 0.2, 0.2, 1.0);

	Camera MainCamera = Camera();
	MainCamera.position.z = -5.0f;

	StandardMaterial Mat = StandardMaterial(&MainCamera, window);

	Node mario = Node();
	
	Mesh* marioMesh = LoadObj("res/mario.obj");
	marioMesh->BindMaterial(&Mat);
	Texture* MarioTexture = new Texture("res/Mario.png");
	
	mario.components.push_back(marioMesh);

	Node light = Node();

	Mesh* lightMesh = LoadObj("res/light.obj");
	lightMesh->BindMaterial(&Mat);
	Texture* LightTexture = new Texture("res/Light.png");

	light.components.push_back(lightMesh);

	glm::vec3 LightPosition(1.0);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 lookAt = glm::lookAt(MainCamera.position, glm::vec3(0), glm::vec3(0, 1.f, 0));
	MainCamera.BindOverrideTransform(&lookAt);

	while (!glfwWindowShouldClose(*window))
	{
		glfwMakeContextCurrent(*window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double x,y;
		if (!Locked)
			glfwGetCursorPos(*window, &SavedX, &SavedY);
		
		x = SavedX;
		y = SavedY;
		x /= window->width;
		y /= window->height;
		x = max(min(x, 1.0), 0.0);
		x *= 10.f;

		MainCamera.position.x = cos(x) * 5.f;
		MainCamera.position.z = sin(x) * 5.f;

		lookAt = glm::lookAt(MainCamera.position, glm::vec3(0), glm::vec3(0, 1.f, 0));

		window->UpdateSize();

		glViewport(0,0, window->width, window->height);

		light.position = LightPosition;
		glm::vec3 actualLight = glm::vec3(light.GetTransform()[3]);
		Mat.BindLightPosition(&actualLight);
		light.scale = glm::vec3(scale / 10.f);

		Mat.SetDiffuse(LightTexture);
		light.update(0.f);
		
		mario.scale = glm::vec3(scale / 10.f);

		Mat.SetDiffuse(MarioTexture);
		mario.update(0.f);

		NewFrame();

		try
		{
			ImGui::Begin("Window");
			ImGui::Checkbox("Lock Camera", &Locked);
			if (ImGui::Button("Reload Shaders"))
			{
			}

			ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPosition), 0.f, 100.f);
			ImGui::End();
		}
		catch (...)
		{
			cout << "ERROR::IMGUI" << endl;
		}

		RenderImgui();

		glfwSwapBuffers(*window);

		while (!focused)
		{
			this_thread::sleep_for(100ms);
		}

		this_thread::sleep_for(1.6ms);
	}
	DestroyImgui();
}