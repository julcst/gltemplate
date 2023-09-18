#version 330 core
out vec4 color;
in vec3 vertexColor;

uniform vec2 uRes;
uniform float uT;

#include "random.glsl"

void main() {
    vec3 uv = vec3(gl_FragCoord.xy / uRes, 0.0) + uT * vec3(0.02, 0.02, 0.0);
    color = vec4(hash3f(floor(uv * 100)), 1.0);
    //color = vec4(gl_FragCoord.xy / uRes, 0.0, 1.0);
}