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

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;



//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;		
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 texCoords;




void main()
{
	vec3 E = normalize(ecViewDir);
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);

	texColor = texColor * texture2D(diffuseTexture, texCoords).rgba;
	vec3 N = normalize( ecNormal);
	vec3 L = normalize(ecLightDir);
	float lambert = max(dot(N, L), 0.0);

	if (lambert > 0.0) {
		FragColor += light.diffuse * texColor * lambert * material.diffuse;
    	vec3 R = normalize(2.0 * dot(N, ecLightDir) * N - ecLightDir);
        float specular = pow(max(dot(R, E), 0.0), material.shininess);
		FragColor += light.specular * texColor * specular * material.specular;
	}
}

