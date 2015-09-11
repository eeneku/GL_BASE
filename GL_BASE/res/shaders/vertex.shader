#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 inTexcoords;

out vec3 vertexColor;
out vec2 texcoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	vertexColor = color;
	texcoords = inTexcoords;
}