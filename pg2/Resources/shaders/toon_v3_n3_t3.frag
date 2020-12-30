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

struct MaterialInfo {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;	
	float transparency;
	float shininess;
	int illumination;
	int hasDiffuseTexture;
};

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;

uniform vec4 warmColor = vec4(0.8, 0.0, 0.1, 1.0);
uniform vec4 coolColor = vec4(0.1, 0.0, 0.8, 1.0);

void main()
{
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);
	float intensity = clamp(dot(N, L), 0.0, 1.0);

	if (intensity > 0.90) {
		FragColor = vec4(0.467, 0.533, 0.600, 1.0);
	} else if (intensity > 0.35) {
		FragColor = vec4(0.439, 0.502, 0.565, 1.0);
	} else {
		FragColor = vec4(0.2, 0.2, 0.2, 1.0);
	}
}
