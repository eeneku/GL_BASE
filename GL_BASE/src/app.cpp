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

	shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	texture = new Texture(GL_TEXTURE_2D, "res/textures/cube.png");

	mesh = new Mesh("res/meshes/cube.mesh");

	glfwSetKeyCallback(window, keyCallback);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->getIndices().size(), mesh->getIndices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::Vertex) * mesh->getVertices().size(), mesh->getVertices().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)(offsetof(Mesh::Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)(offsetof(Mesh::Vertex, uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)(offsetof(Mesh::Vertex, normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
}

App::~App()
{
	delete shader;

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

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(4.0f, 10.0f, -30.0f),
		glm::vec3(-3.0f, -4.4f, -5.0f),
		glm::vec3(-7.6f, -4.0f, -24.6f),
		glm::vec3(4.8f, -0.8f, -7.0f),
		glm::vec3(-3.4f, 6.0f, -15.0f),
		glm::vec3(2.6f, -4.0f, -5.0f),
		glm::vec3(3.0f, 4.0f, -5.0f),
		glm::vec3(3.0f, 0.4f, -3.0f),
		glm::vec3(-2.6f, 2.0f, -3.0f)
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->bind();

	texture->bind(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->getProgram(), "texture"), 0);

	glBindVertexArray(VAO);

	for (size_t i = 0; i < 10; i++)
	{
		glm::mat4 model2;
		model2 = glm::translate(model2, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(projection * camera.getView() * model2));
		glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, nullptr);
	}

	glBindVertexArray(0);

	shader->unbind();

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