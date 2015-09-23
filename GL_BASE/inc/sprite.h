#ifndef SPRITE_H
#define SPRITE_H

#include "glm\glm.hpp"
#include "texture.h"

class Sprite
{
public:
	Sprite(glm::vec3 position, Texture* texture);
	~Sprite();

	void render();
private:

	Texture* texture;
	glm::vec3 position;
};

#endif