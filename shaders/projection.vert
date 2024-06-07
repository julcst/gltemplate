#version 330 core

/* Specify layout of the input vertex data */
layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _uv;
layout (location = 2) in vec3 _normal;
layout (location = 3) in vec3 _tangent;

/* Specify output of the vertex shader */
out VertexData {
    vec2 uv;
    vec3 localPosition;
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
};

/* We outsource the definition of the uniforms to a separate file to avoid repetition. */
#include "uniforms.glsl"
/* The #line XX YYY macro is used for correct error messages, errors will be shown as "ERROR: YYY:??: ..." in terminal.
   This is necessary because the #include macro messes up our line numbering. Specify the number of the next line with XX and the file with YYY. */
#line 23 104

/**
 * Applies the model, view, and projection matrices to a mesh
 */
void main() {

    // Apply model, view and projection transformation
    gl_Position = uLocalToClip * vec4(_position, 1.0);

    // Pass uv coordinates to fragment shader
    uv = _uv;
    // Pass object space position to fragment shader
    localPosition = _position;
    
    // Transform normals and tangents to world space
    worldPosition = (uLocalToWorld * vec4(_position, 1.0)).xyz;
    worldNormal = normalize((uLocalToWorld * vec4(_normal, 0.0)).xyz);
    worldTangent = normalize((uLocalToWorld * vec4(_tangent, 0.0)).xyz);
    // Gram-Schmidt orthogonalization to reorthogonalize the interpolated tangent space
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);
}