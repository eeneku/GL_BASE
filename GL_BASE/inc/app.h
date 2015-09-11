#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "shader.h"
#include "texture.h"

class App
{
public:
	App();
	~App();

	void run();
private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void render();

	GLFWwindow* window;
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	GLuint programID;
	Shader* shader;
	Texture* texture1;
	Texture* texture2;
};

#endif