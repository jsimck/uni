#version 400 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
layout(location = 2) out float DepthColor;

uniform float life;
uniform vec3 color;
uniform float near;
uniform float far;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	FragColor = vec4(color, life);
	BrightColor = vec4(color, life);
    DepthColor = LinearizeDepth(gl_FragCoord.z);
}