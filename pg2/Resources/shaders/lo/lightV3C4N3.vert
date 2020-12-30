#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTex;

uniform mat4 MVPatrix;

void main() {
	gl_Position = MVPatrix * vec4(VertexPosition, 1.0);
}