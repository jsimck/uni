#version 400 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloom;

in vec2 texCoords;

void main()
{
	vec3 sceneColor = texture(scene, texCoords).rgb;
	vec3 bloomColor = texture(bloom, texCoords).rgb;
	vec3 result = sceneColor + bloomColor;

	// Gamma correction
	const float gamma = 0.9;
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}