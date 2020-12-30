#version 400 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTex;

out vec4 color;
out vec2 texCoords;

void main()
{
	color = VertexColor;
	texCoords = VertexTex;
	gl_Position = vec4(VertexPosition, 1.0);
	//gl_Position = PMatrix * MVMatrix * VertexPosition;
}