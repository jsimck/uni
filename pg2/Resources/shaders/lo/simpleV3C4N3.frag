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

struct MaterialInfo {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;	
	float transparency;
	float shininess;
	int illumination;
	int hasDiffuseTexture;
};

layout (location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D emissionTex;

in vec4 ecPosition;
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 texCoords;

void main() {
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);
	vec3 E = normalize(ecViewDir);
	vec3 R = normalize(reflect(-L, ecNormal));

	float lambert = max(dot(N, L), 0.0);
	float spec = pow(max(dot(R, E), 0.0), material.shininess);

	vec4 ambient = texture(diffuseTex, texCoords) * light.ambient;
	vec4 diffuse = lambert * texture(diffuseTex, texCoords) * light.diffuse;
	vec4 specular = spec * texture(specularTex, texCoords) * light.specular;

	FragColor = ambient + diffuse + specular + texture(emissionTex, texCoords);
}