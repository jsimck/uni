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
uniform sampler2D diffuseTexture;
uniform float shift;
uniform float pulse;
uniform float near;
uniform float far;

in vec2 texCoords;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	vec4 mixedTexture = mix(
		texture(diffuseTexture, vec2(texCoords.x + shift * 2, texCoords.y + shift)), 
		texture(diffuseTexture, vec2(texCoords.x - 0.5 - shift * 2, texCoords.y - 0.5 - shift)), 
		0.5
	);

	FragColor = vec4(mix(mixedTexture, material.diffuse, 0.3).rgb * pulse, 1.0);
    BrightColor = vec4(material.diffuse.rgb, pulse);
    DepthColor = LinearizeDepth(gl_FragCoord.z);
}
