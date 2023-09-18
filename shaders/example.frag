#version 330 core
out vec4 color;
in vec3 vertexColor;

#include "random.glsl"

void main() {
    vec2 uv = ((gl_FragCoord.xy / vec2(800, 600)) - 1.0) * 20.0;
    color = vec4(hash3f(vec3(floor(gl_FragCoord / 20.0))), 1.0);
}