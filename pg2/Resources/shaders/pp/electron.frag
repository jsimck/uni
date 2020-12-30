#version 400 core //compatibility

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
layout(location = 2) out float DepthColor;

uniform MaterialInfo material;
uniform float near;
uniform float far;

in vec3 ecNormal;
in vec3 ecViewDir;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	vec3 N = normalize(ecNormal);
	vec3 E = normalize(ecViewDir);

	float intensity = clamp(dot(E, N), 0.0, 1.0);
	
	FragColor = mix(material.diffuse, material.specular, intensity);
    BrightColor = material.diffuse;
    DepthColor = LinearizeDepth(gl_FragCoord.z);
}
