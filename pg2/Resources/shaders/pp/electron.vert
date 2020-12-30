#version 400 core //compatibility

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)

out vec3 ecViewDir;
out vec3 ecNormal;

void main()
{
	vec4 ecPosition = MVMatrix * vec4(VertexPosition, 1.0);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);

	gl_Position = PMatrix * ecPosition;
}
