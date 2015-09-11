#version 330 core

in vec3 vertexColor;
in vec2 texcoords;

out vec4 fragmentColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	fragmentColor = mix(texture2D(texture1, vec2(texcoords.x, -texcoords.y)), 
						texture2D(texture2, vec2(texcoords.x, -texcoords.y)), 0.4f) * vec4(vertexColor, 1.0f);
}