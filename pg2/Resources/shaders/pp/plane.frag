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
layout(location = 2) out float DepthColor;

#define LIGHTS_COUNT 4
uniform LightInfo light[LIGHTS_COUNT];
uniform MaterialInfo material;
uniform float pulse;
uniform float near;
uniform float far;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;		
in vec3 ecLightDir[LIGHTS_COUNT];
in vec3 ecNormal;
in vec3 ecViewDir;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec4 calcPointLight(int i, vec3 N, vec3 L, vec3 E, LightInfo light) {
	vec3 R = normalize(reflect(-L, N));

	float lightPulse = 1.0;
	float diff = 1.0;
	float lambert = max(dot(N, L), 0.0);
	float spec = pow(max(dot(R, E), 0.0), material.shininess / light.intensity);

	// Claculate attenuation (light intensity fade with distance), only for electrons
	float attenuation = 1.0;
	if (i > 0) {
		float distance = length(light.position - ecPosition);
		attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		diff = pow(max(dot(R, E), 0.0), 8);
	} else {
		diff = pow(max(dot(R, E), 0.0), material.shininess / light.intensity);
		lightPulse = pulse, 0;
	}

	vec4 ambient = material.ambient * light.ambient;
	vec4 diffuse = diff * lambert * material.diffuse * (light.diffuse * lightPulse);
	vec4 specular = spec * material.specular * (light.specular * lightPulse);

	// Attenuation = (light intensity fade with distance) 
	if (i > 0) {
		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
	}

	return specular + ambient + diffuse;
}

void main() {
	vec3 N = normalize(ecNormal);
	vec3 E = normalize(ecViewDir);
	vec4 result;

	for (int i = 0; i < LIGHTS_COUNT; i++) {
		vec3 L = normalize(ecLightDir[i]);
		result += calcPointLight(i, N, L, E, light[i]);
	}

	FragColor = result;
    DepthColor = LinearizeDepth(gl_FragCoord.z);
}
