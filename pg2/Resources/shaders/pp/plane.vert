#version 400 core //compatibility

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlight
	float spotCutoff;			//Specifies maximum spread angle of spotlight (180 = off).
	float intensity;
	float constant;
	float linear;
	float quadratic;
};

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)

#define LIGHTS_COUNT 4
uniform LightInfo light[LIGHTS_COUNT];

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out vec4 ecPosition;
out vec3 ecLightDir[LIGHTS_COUNT];
out vec3 ecNormal;
out vec3 ecViewDir;

void main()
{
	ecPosition = MVMatrix * vec4(VertexPosition, 1.0);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);

	// Light positions
	for (int i = 0; i < LIGHTS_COUNT; i++) {
		ecLightDir[i] = vec3((VMatrix * light[i].position) - ecPosition);
	}

	gl_Position = PMatrix * ecPosition;
}
