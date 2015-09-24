#ifndef SPRITE_H
#define SPRITE_H

#include "glm\glm.hpp"
#include "GL\glew.h"

class Texture;
class Shader;

class Sprite
{
public:
	Sprite(glm::vec3 position, Texture* texture, Shader* shader);
	~Sprite();

	void render(const glm::mat4& view, const glm::mat4& projection);
private:

	Texture* texture;
	Shader* shader;
	glm::vec3 position;
	GLuint VBO;
	GLuint IBO;
	GLuint uniformMVP;
	GLuint uniformTexture;

	glm::mat4 model;
};

#endif