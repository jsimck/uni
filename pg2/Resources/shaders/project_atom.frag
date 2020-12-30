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
layout(location = 1) out vec4 BrightColor;

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;		
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 ecTexCoords;

void main()
{
	vec4 cool = vec4(0, 0.3, 0.7, 1.0);
	vec4 warm = vec4(0, 0.7, 1.0, 1.0);

	vec3 N = normalize(ecNormal);
	vec3 E = normalize(ecViewDir);
	float intensity = clamp(dot(E, N), 0.0, 1.0);

	FragColor = mix(cool, warm, intensity);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.3) {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
}
