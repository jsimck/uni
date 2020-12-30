#version 400 core //compatibility

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlight
	float spotCutoff;			//Specifies maximum spread angle of spotlight (180 = off).
};

in vec4 color;

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;

void main()
{
	if(dot(ecLightDir, ecNormal) > 0.0) {
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	} else{
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
}
