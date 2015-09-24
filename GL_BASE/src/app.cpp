#include "app.h"

#include <cassert>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

App::App() : window(nullptr), shader(nullptr), camera(glm::vec3(0.0f, 0.0f, 3.0f))
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "GL BASE", nullptr, nullptr);

	assert(window != nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	assert(glewInit() == GLEW_OK);

	glViewport(0, 0, 800, 600);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	texture = new Texture(GL_TEXTURE_2D, "res/textures/penguins.png");
	texture2 = new Texture(GL_TEXTURE_2D, "res/textures/wall.png");
	sprite = new Sprite(glm::vec3(0.0f, 0.0f, 0.0f), texture, shader);
	sprite2 = new Sprite(glm::vec3(250.0f, 250.0f, 1.0f), texture2, shader);

	glfwSetKeyCallback(window, keyCallback);

	projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 100.0f);
}

App::~App()
{
	glBindVertexArray(0);

	delete shader;
	delete texture;
	delete sprite;

	glfwTerminate();
}

void App::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		render();
	}
}

void App::render()
{
	checkMovement();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sprite->render(camera.getView(), projection);
	sprite2->render(camera.getView(), projection);

	glfwSwapBuffers(window);
}

void App::checkMovement()
{
	GLfloat speed = 0.05f;

	if (keys[GLFW_KEY_W])
		camera.setPosition(camera.getPosition() + camera.getFront() * speed);
	if (keys[GLFW_KEY_S])
		camera.setPosition(camera.getPosition() - camera.getFront() * speed);
	if (keys[GLFW_KEY_A])
		camera.setPosition(camera.getPosition() - camera.getRight() * speed);
	if (keys[GLFW_KEY_D])
		camera.setPosition(camera.getPosition() + camera.getRight() * speed);
	if (keys[GLFW_KEY_Q])
		camera.setYawPitch(-0.5f, 0.0f);
	if (keys[GLFW_KEY_E])
		camera.setYawPitch(0.5f, 0.0f);
	if (keys[GLFW_KEY_Z])
		camera.setYawPitch(0.0f, +0.5f);
	if (keys[GLFW_KEY_X])
		camera.setYawPitch(0.0f, -0.5f);
}

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS) keys[key] = true;
	else if (action == GLFW_RELEASE) keys[key] = false;
}