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
	void unbind();

	GLuint getId() { return texture; }
	GLuint getWidth() { return width; }
	GLuint getHeight() { return height; }
private:
	GLuint texture;
	GLuint width;
	GLuint height;
	GLenum target;
};

#endif