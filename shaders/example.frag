#version 330 core
out vec4 fragColor;

uniform vec2 uRes;
uniform float uT;

#include "random.glsl"

void main() {
    vec2 uv = gl_FragCoord.xy / uRes;
    if (hash3f(vec3(floor(gl_FragCoord.xy / 1.0), uT)).x > 0.5) discard;
    switch (int(floor(mod(uT, 3.0)))) {
    case 0:
        vec3 p = vec3(gl_FragCoord.xy / uRes, 0.0) + uT * vec3(0.02, 0.02, 0.0);
        fragColor = vec4(hash3f(floor(p * 100)), 1.0);
        break;
    case 1:
        fragColor = vec4(vec3(uv.x * uv.y), 1.0);
        break;
    case 2:
        fragColor = vec4(uv, 0.0, 1.0);
        break;
    }
}