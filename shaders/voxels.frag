#version 330 core

in VertexData {
    vec2 uv;
    vec3 localPosition;
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
};

out vec4 fragColor;

#include "random.glsl"

void main() {
    fragColor = vec4(hash3f(floor(localPosition * 10.0001)).xxx, 1.0);
}