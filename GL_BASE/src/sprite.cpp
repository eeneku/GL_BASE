#include "sprite.h"

#include "texture.h"
#include "shader.h"

#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"

Sprite::Sprite(glm::vec3 position, Texture* texture, Shader* shader) : position(position), texture(texture), shader(shader), model(1.0f)
{
	GLfloat width = (GLfloat)texture->getWidth();
	GLfloat height = (GLfloat)texture->getHeight();

	GLfloat vertices[] = {
		0.0f, height, 0.0f,		0.0f, 1.0f,
		width, height, 0.0f,	1.0f, 1.0f,
		width, 0.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	model = glm::translate(model, position);
}

Sprite::~Sprite()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Sprite::render(const glm::mat4& view, const glm::mat4& projection)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shader->bind();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glUniform1i(uniformTexture, 0);
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection * view * model));

	texture->bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	shader->unbind();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}