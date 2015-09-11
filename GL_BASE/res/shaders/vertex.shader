#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 inTexcoords;

out vec3 vertexColor;
out vec2 texcoords;

uniform vec3 yoloColor;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1.0f);
	vertexColor = color * yoloColor;
	texcoords = inTexcoords;
}