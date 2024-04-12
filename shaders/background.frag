#version 330 core

in vec3 viewDir;
out vec3 fragColor;

#include "uniforms.glsl"
#line 8 101

void main() {
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 sky = exp(-abs(rayDir.y) / vec3(0.1, 0.3, 0.6));
    fragColor = sky;
}