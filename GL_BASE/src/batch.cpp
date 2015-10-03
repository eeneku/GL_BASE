#include "batch.h"
#include "shader.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include <cassert>

Batch::Batch(Shader* shader, size_t maxVertices, size_t maxIndices) : shader(shader), hasBegun(false), currentIndex(0), currentVertex(0)
{
	assert(maxVertices > 0);

	vertices.resize(maxVertices);

	projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 100.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (maxIndices > 0)
	{
		indices.resize(maxIndices);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


Batch::~Batch()
{
	glDeleteBuffers(1, &VBO);

	if (indices.size() > 0)
		glDeleteBuffers(1, &IBO);

	vertices.clear();
	indices.clear();
}

void Batch::begin()
{
	assert(!hasBegun);

	hasBegun = true;
}

void Batch::end()
{
	assert(hasBegun);

	flush();

	hasBegun = false;
	currentVertex = 0;
	currentIndex = 0;
}

void Batch::draw(const std::vector<Vertex>* vertices, const std::vector<GLuint>* indices)
{
	assert(hasBegun);

	if (indices && indices->size() > 0)
	{
		for (auto index : *indices)
			this->indices[currentIndex++] = currentVertex + index;
	}

	if (vertices && vertices->size() > 0)
	{ 
		for (auto vertex : *vertices)
			this->vertices[currentVertex++] = vertex;
	}
	else
	{ 
		return;
	}
}

void Batch::flush()
{
	if (currentVertex > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, position)));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, color)));

		shader->bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * currentVertex, (void*)(vertices.data()));

		GLuint uniformMVP = glGetUniformLocation(shader->getProgram(), "MVP");

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection));

		if (indices.size() > 0 && currentIndex > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * currentIndex, (void*)(indices.data()));

			glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, currentVertex);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		shader->unbind();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Batch::drawRectangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(position.x, position.y + size.y, position.z), color),
		Vertex(glm::vec3(position.x + size.x, position.y + size.y, position.z), color),
		Vertex(glm::vec3(position.x + size.x, position.y, position.z), color),
		Vertex(glm::vec3(position.x, position.y, position.z), color)};

	std::vector<GLuint> indices = {
		0, 1, 2, 0, 2, 3
	};

	draw(&vertices, &indices);
}