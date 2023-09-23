#version 330 core
in vec3 pos;
out vec4 fragColor;

#include "random.glsl"

void main() {
    fragColor = vec4(hash3f(floor(pos * 10.0001)).xxx, 1.0);
}