#ifndef APP_H
#define APP_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "sprite.h"
#include "batch.h"

static bool keys[1024];

class App
{
public:
	App();
	~App();

	void run();
private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void render();
	void checkMovement();
	void renderTexture(Texture* texture, GLfloat x, GLfloat y);

	GLFWwindow* window;
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	GLuint programID;
	Shader* shader;
	Shader* primitiveShader;
	Texture* texture;
	Texture* texture2;
	Camera camera;
	Sprite* sprite;
	Sprite* sprite2;
	Batch* batch;

	glm::mat4 projection;
};

#endif