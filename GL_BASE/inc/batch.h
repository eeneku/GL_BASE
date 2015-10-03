#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

class Shader;

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;

	Vertex(const glm::vec3& position, const glm::vec4& color) : position(position), color(color) {}
	Vertex(Vertex&& other) { this->position = std::move(other.position); this->color = std::move(other.color); }
	Vertex() : position(0.0f), color(0.0f) {}
};

class Batch
{
public:
	Batch(Shader* shader, size_t maxVertices = defaultBatchSize, size_t maxIndices = defaultBatchSize * 3);
	~Batch();

	void begin();
	void end();
	void draw(const std::vector<Vertex>* vertices, const std::vector<GLuint>* indices = nullptr);
	void drawRectangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	void flush();
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	static const size_t defaultBatchSize = 2048;

	GLuint IBO;
	GLuint VBO;
	GLuint currentVertex;
	GLuint currentIndex;

	glm::mat4 projection;

	bool hasBegun;

	Shader* shader;
};