#version 400 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 VertexTex;

out vec2 texCoords;

void main()
{
	texCoords = VertexTex;
	gl_Position = vec4(VertexPosition, 1.0);
}