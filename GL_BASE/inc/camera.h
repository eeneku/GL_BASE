#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "GL/glew.h"

class Camera
{
public:
	Camera(glm::vec3 position);
	~Camera();

	void setPosition(glm::vec3 v) { position = v; calculateVectors(); }
	void setYawPitch(GLfloat y, GLfloat p);

	const glm::vec3& getPosition() const { return position; }
	const glm::vec3& getFront() const { return front; }
	const glm::vec3& getUp() const { return up; }
	const glm::vec3& getRight() const { return right; }
	const glm::mat4& getView() const { return view; }
private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	glm::mat4 view;

	void calculateVectors();
};

#endif