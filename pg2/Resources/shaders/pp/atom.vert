#version 400 core //compatibility

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec3 VertexTex;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix

out vec2 texCoords;

void main()
{
	texCoords = VertexTex.xy;
	gl_Position = PMatrix * MVMatrix * vec4(VertexPosition, 1.0);
}
