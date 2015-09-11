#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(GLenum target, const std::string& path);
	~Texture();

	void bind(GLenum to);

	GLuint getId() { return texture; }
private:
	GLuint texture;
	GLuint width;
	GLuint height;
	GLenum target;
};

#endif