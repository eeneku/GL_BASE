#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos) : position(pos), front(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(-90.0f), pitch(0.0f), worldUp(up)
{	
	calculateVectors();
}

Camera::~Camera()
{
}

void Camera::calculateVectors()
{
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	view = glm::lookAt(position, position + front, up);
}

void Camera::setYawPitch(GLfloat y, GLfloat p)
{
	pitch += p; 
	if (pitch > 89.0f) 
		pitch = 89.0f; 
	if (pitch < -89.0f) 
		pitch = -89.0f; 

	yaw += y; 
	
	calculateVectors();
}