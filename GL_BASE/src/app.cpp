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

	window = glfwCreateWindow(1280, 720, "GL BASE", nullptr, nullptr);

	assert(window != nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	assert(glewInit() == GLEW_OK);

	glViewport(0, 0, 1280, 720);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	primitiveShader = new Shader("res/shaders/primitive.vertex.shader", "res/shaders/primitive.fragment.shader");
	texture = new Texture(GL_TEXTURE_2D, "res/textures/penguins.png");
	texture2 = new Texture(GL_TEXTURE_2D, "res/textures/wall.png");
	sprite = new Sprite(glm::vec3(0.0f, 0.0f, 0.0f), texture, shader);
	sprite2 = new Sprite(glm::vec3(250.0f, 250.0f, 1.0f), texture2, shader);
	batch = new Batch(primitiveShader, 2048);

	glfwSetKeyCallback(window, keyCallback);

	projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 100.0f);
}

App::~App()
{
	glBindVertexArray(0);

	delete shader;
	delete primitiveShader;
	delete texture;
	delete texture2;
	delete sprite;
	delete sprite2;

	glfwTerminate();
}

void App::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		checkMovement();

		render();
	}
}

void App::render()
{
	batch->begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	batch->drawRectangle(glm::vec3(10.0f, 10.0f, 1.0f), glm::vec2(100.0f, 100.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	batch->drawRectangle(glm::vec3(100.0f, 100.0f, 1.0f), glm::vec2(250.0f, 75.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//renderTexture(texture2, 100.0f, 100.0f);

	batch->end();

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

void App::renderTexture(Texture* texture, GLfloat x, GLfloat y)
{
	GLfloat width = (GLfloat)texture->getWidth();
	GLfloat height = (GLfloat)texture->getHeight();
	GLuint IBO, VBO, uniformMVP, uniformTexture;

	glm::mat4 model(1.0f);

	GLfloat vertices[] = {
		0.0f, height, 0.0f, 0.0f, 1.0f,
		width, height, 0.0f, 1.0f, 1.0f,
		width, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3
	};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	uniformMVP = glGetUniformLocation(shader->getProgram(), "MVP");
	uniformTexture = glGetUniformLocation(shader->getProgram(), "texture");

	model = glm::translate(model, glm::vec3(x, y, 0.0f));

	shader->bind();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glUniform1i(uniformTexture, 0);
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection * camera.getView() * model));

	texture->bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	shader->unbind();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}