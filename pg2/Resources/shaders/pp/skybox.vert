#version 400 core

layout(location = 0) in vec3 VertexPosition;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix

out vec3 TexCoords;

void main()
{
	TexCoords = VertexPosition;
	gl_Position = PMatrix * MVMatrix * vec4(VertexPosition, 1.0);
}