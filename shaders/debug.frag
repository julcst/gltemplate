#version 330 core

in VertexData {
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
};
out vec3 fragColor;

#include "debug.glsl"
#line 13 102

/**
 * Renders the normals and a uv checkerboard pattern onto the mesh
 */
void main() {
    vec3 N = normalize(worldNormal); // Renormalize after interpolation
    fragColor = normalToRGB(N) * uvCheckerboard(uv, 100.0);
}