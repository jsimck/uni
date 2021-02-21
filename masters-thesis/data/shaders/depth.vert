#version 400 core

layout (location = 0) in vec3 aPos;

out float depth;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

void main() {
    gl_Position = MVPMatrix * vec4(aPos, 1.0);
    vec3 vEyePos = (MVMatrix * vec4(aPos, 1.0)).xyz;

    // OpenGL Z axis goes out of the screen, so depths are negative
    depth = -vEyePos.z;
}