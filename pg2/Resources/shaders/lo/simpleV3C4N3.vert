#version 400 core

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlighit
	float spotCutoff;		//Specifies maximum spread angle of spotlight (180 = off).
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTex;

uniform LightInfo light;
uniform vec3 lightPosition;

uniform mat4 PMatrix;
uniform mat4 VMatrix;
uniform mat4 MMatrix;
uniform mat4 MVatrix;
uniform mat3 NormalMatrix;

out vec4 ecPosition;
out vec3 ecLightDir;
out vec3 ecNormal;
out vec3 ecViewDir;
out vec2 texCoords;

void main() {
	ecPosition = VMatrix * MMatrix * vec4(VertexPosition, 1.0);
	ecLightDir = vec3((VMatrix * light.position) - ecPosition);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);
	texCoords = VertexTex;

	gl_Position = PMatrix * ecPosition;
}