#include "app.h"

#include <cassert>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

App::App() : window(nullptr), shader(nullptr)
{
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

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

	shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	texture1 = new Texture(GL_TEXTURE_2D, "res/textures/wall.png");
	texture2 = new Texture(GL_TEXTURE_2D, "res/textures/awesomeface.png");

	glfwSetKeyCallback(window, keyCallback);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


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
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat timeValue = glfwGetTime();

	shader->bind();

	glm::mat4 MVP;
	MVP = glm::rotate(MVP, glm::radians(timeValue * 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	MVP = glm::scale(MVP, glm::vec3(0.4f, 0.4f, 1.0f));
	MVP = glm::translate(MVP, glm::vec3(0.75f, -0.75f, 0.0f));
	MVP = glm::rotate(MVP, glm::radians(timeValue * 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	

	texture1->bind(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->getProgram(), "texture1"), 0);

	texture2->bind(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(shader->getProgram(), "texture2"), 1);

	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glUniform3f(glGetUniformLocation(shader->getProgram(), "yoloColor"), (sin(timeValue) / 2) + 0.3f, (-sin(timeValue) / 2) + 0.2f, (sin(timeValue) / 2) + 0.9f);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	shader->unbind();

	glfwSwapBuffers(window);
}

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}