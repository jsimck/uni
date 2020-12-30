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

uniform vec4 coolColor = vec4(0.0, 0.0, 1.0, 1.0);
uniform vec4 warmColor = vec4(1.0, 0.0, 0.0, 1.0);
uniform float alpha = 0.3;
uniform float beta = 0.7;

void main()
{
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);
	vec3 E = normalize(ecViewDir);
	float edge = dot(N, E);
	
	// Black edge
	if (edge < 0.2 && edge > -0.2) {
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		// Gooch
		vec4 kc = coolColor + alpha * material.diffuse;
		vec4 kw = warmColor + beta * material.diffuse;

		// Alternative
		//FragColor = ((1 + edge) / 2) * kc + (1 - (1 + edge) / 2) * kw;
		FragColor = mix(kw, kc, edge);

		// Specular
        vec3 R = normalize(reflect(-E, N));    
        FragColor += pow(max(dot(R, E),0.0), material.shininess) * light.specular * material.specular;

		// Ambient
		// FragColor += material.ambient * light.ambient;
	}
}
