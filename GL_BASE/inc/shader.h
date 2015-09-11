#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void bind();
	void unbind();
	GLuint getProgram() { return program; }
private:
	GLuint program;
};

#endif