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
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	view = glm::lookAt(position, position + front, up);
}