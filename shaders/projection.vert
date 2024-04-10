#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _uv;
layout (location = 2) in vec3 _normal;
layout (location = 3) in vec3 _tangent;
layout (location = 4) in vec3 _bitangent;

out VertexData {
    vec2 uv;
    vec3 localPosition;
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
};

#include "uniforms.glsl"
#line 18 108

/**
 * Applies the model, view, and projection matrices to a mesh
 */
void main() {

    // Apply perspective transformation
    gl_Position = uLocalToClip * vec4(_position, 1.0);

    // Pass uv coordinates to fragment shader
    uv = _uv;

    localPosition = _position;
    
    // Transform normals and tangents to world space
    worldPosition = (uLocalToWorld * vec4(_position, 1.0)).xyz;
    worldNormal = normalize((uLocalToWorld * vec4(_normal, 0.0)).xyz);
    worldTangent = normalize((uLocalToWorld * vec4(_tangent, 0.0)).xyz);
    // Gram-Schmidt orthogonalization
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);
}