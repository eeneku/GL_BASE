#include "texture.h"
#include "lodepng.h"

#include <vector>

Texture::Texture(GLenum target, const std::string& path) : target(target)
{
	std::vector<unsigned char> file;
	std::vector<unsigned char> pixels;

	lodepng::load_file(file, path);
	lodepng::decode(pixels, width, height, file.data(), file.size());

	glGenTextures(1, &texture);
	glBindTexture(target, texture);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::bind(GLenum to)
{
	glActiveTexture(to);
	glBindTexture(target, texture);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(target, 0);
}