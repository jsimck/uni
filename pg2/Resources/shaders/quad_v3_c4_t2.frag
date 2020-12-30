#version 400 core

layout(location = 0) out vec4 FragColor;
uniform sampler2D frameBufferTexture;

in vec4 color;
in vec2 texCoords;

const float offset = 1.0 / 500.0;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
	);

	// float kernel[9] = float[](
	// 	-1.0, -1.0, -1.0,
	// 	-1.0,  9.0, -1.0,
	// 	-1.0, -1.0, -1.0
	// );

	// float kernel[9] = float[](
 //    1.0 / 16, 2.0 / 16, 1.0 / 16,
 //    2.0 / 16, 4.0 / 16, 2.0 / 16,
 //    1.0 / 16, 2.0 / 16, 1.0 / 16  
	// );

	float kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);

	vec3 kColor = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		kColor += vec3(texture(frameBufferTexture, texCoords + offsets[i])) * kernel[i];
	}

	FragColor = vec4(kColor, 1.0);
}