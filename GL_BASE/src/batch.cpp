#include "batch.h"
#include "shader.h"
#include "color.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include <iostream>
#include <cassert>

Batch::Batch(Shader* shader, size_t maxVertices, size_t maxIndices) : shader(shader), hasBegun(false), currentIndex(0), currentVertex(0), currentlyIndexing(false)
{
	assert(maxVertices > 0);

	vertices.resize(maxVertices);

	projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -100.0f, 100.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxVertices, vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (maxIndices > 0)
	{
		indices.resize(maxIndices);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * maxIndices, indices.data(), GL_DYNAMIC_DRAW);
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

	std::cout << "Batch begin" << std::endl;

	hasBegun = true;
}

void Batch::end()
{
	assert(hasBegun);

	flush();

	std::cout << "Batch end" << std::endl << std::endl;

	hasBegun = false;
}

void Batch::draw(const std::vector<Vertex>* vertices, const std::vector<GLuint>* indices)
{
	assert(hasBegun);

	if (indices && indices->size() > 0)
	{
		if (!currentlyIndexing)
		{
			flush();
			currentlyIndexing = true;
		}

		for (auto index : *indices)
			this->indices[currentIndex++] = currentVertex + index;
	}
	else if (currentlyIndexing)
	{
		flush();
		currentlyIndexing = false;
	}

	if (vertices && vertices->size() > 0)
	{ 
		for (auto vertex : *vertices)
			this->vertices[currentVertex++] = vertex;
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
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * currentVertex, (GLvoid*)(vertices.data()));

		GLuint uniformMVP = glGetUniformLocation(shader->getProgram(), "MVP");

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection));

		if (currentIndex > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * currentIndex, (GLvoid*)(indices.data()));

			glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			currentIndex = 0;
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, currentVertex);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		shader->unbind();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		currentVertex = 0;

		std::cout << "...flush" << std::endl;
	}
}

void Batch::drawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, const Color& color, GLfloat depth)
{
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(x, y + height, depth), glm::vec4(color.r, color.g, color.b, color.a)),
		Vertex(glm::vec3(x + width, y + height, depth), glm::vec4(color.r, color.g, color.b, color.a)),
		Vertex(glm::vec3(x + width, y, depth), glm::vec4(color.r, color.g, color.b, color.a)),
		Vertex(glm::vec3(x, y, depth), glm::vec4(color.r, color.g, color.b, color.a)) };

	std::vector<GLuint> indices = {
		0, 1, 2, 0, 2, 3
	};

	draw(&vertices, &indices);
}

void Batch::drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, const Color& color, GLfloat depth)
{
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(x1, y1, depth), glm::vec4(color.r, color.g, color.b, color.a)),
		Vertex(glm::vec3(x2, y2, depth), glm::vec4(color.r, color.g, color.b, color.a)),
		Vertex(glm::vec3(x3, y3, depth), glm::vec4(color.r, color.g, color.b, color.a)) };

	draw(&vertices);
}