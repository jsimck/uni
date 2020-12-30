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

void main()
{
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);

	float lambert = dot(N, L);
	FragColor = material.ambient * light.ambient;
	
	vec3 E = normalize(ecViewDir);
    vec3 R = normalize(reflect(-L, N));    

	vec4 diffuse = abs(lambert) * material.diffuse * light.diffuse;
    vec4 specular = pow(abs(dot(R, E)), material.shininess) * light.specular * material.specular;

	FragColor += diffuse + specular;
}
