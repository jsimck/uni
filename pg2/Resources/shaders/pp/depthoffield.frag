#version 400 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D blur;
uniform sampler2D depth;

in vec2 texCoords;

void main()
{
    FragColor = mix(texture(scene, texCoords), texture(blur, texCoords), texture(depth, texCoords).r);
    // FragColor = vec4(vec3(texture(depth, texCoords).r), 1.0);
    // FragColor = vec4(vec3(texture(scene, texCoords).r), 1.0);
    // FragColor = vec4(vec3(texture(blur, texCoords).r), 1.0);
}