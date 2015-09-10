#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

GLFWwindow* window = nullptr;
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};
GLuint VBO;
GLuint VAO;
GLuint programID;

int init();
void render();

std::string loadShader(const char* path);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char**argv)
{
	if (init())
	{

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			render();
		}
	}

	glfwTerminate();

	return 0;
}

int init()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "GL BASE", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glfwSetKeyCallback(window, keyCallback);

	GLuint vertexShader;
	GLuint fragmentShader;

	std::string temp1 = loadShader("res/shaders/vertex.shader");
	std::string temp2 = loadShader("res/shaders/fragment.shader");

	const char* vertexShaderSource = temp1.c_str();
	const char* fragmentShaderSource = temp2.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glUseProgram(programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	
	return 1;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(programID);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(window);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

std::string loadShader(const char* path)
{
	std::ifstream file;
	std::string temp;

	file.open(path);

	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;

			std::getline(file, line);

			temp.append(line + "\n");
		}

		std::cout << temp;

		file.close();
	}

	assert(!temp.empty());

	return temp;
}