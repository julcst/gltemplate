#version 330 core
out vec4 color;
in vec3 vertexColor;

uniform vec2 uRes;
uniform float uT;

#include "random.glsl"

void main() {
    switch (int(floor(mod(uT, 3.0)))) {
    case 0:
        vec3 p = vec3(gl_FragCoord.xy / uRes, 0.0) + uT * vec3(0.02, 0.02, 0.0);
        color = vec4(hash3f(floor(p * 100)), 1.0);
        break;
    case 1:
        vec2 uv = gl_FragCoord.xy / uRes;
        color = vec4(vec3(uv.x * uv.y), 1.0);
        break;
    case 2:
        color = vec4(vertexColor, 1.0);
        break;
    }
}