#version 400 core //compatibility

layout(location = 0) out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D scene;
uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
	vec2 texOffset = 1.0 / textureSize(scene, 0); // size of single texel
	vec3 color = texture(scene, texCoords).rgb * weight[0];

    if(horizontal) {
        for(int i = 1; i < 5; ++i) {
           color += texture(scene, texCoords + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
           color += texture(scene, texCoords - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 5; ++i) {
            color += texture(scene, texCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
            color += texture(scene, texCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(color, 1.0);
}