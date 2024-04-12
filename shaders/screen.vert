#version 330 core

/* Specify layout of the input vertex data */
layout (location = 0) in vec3 _position;

/* The output of the vertex shader */
out vec3 viewDir;

/* We outsource the definition of the uniforms to a separate file to avoid repetition. */
#include "uniforms.glsl"
/* The #line XX YYY macro is used for correct error messages, errors will be shown as "ERROR: YYY:??: ..." in terminal.
   This is necessary because the #include macro messes up our line numbering. Specify the number of the next line with XX and the file with YYY. */
#line 14 106

/**
 * Calculates the ray direction for every vertex, which is then hardware interpolated to calculate the per pixel ray direction
 */
void main() {
    /* gl_Position is a built-in output variable that holds the position of a vertex in clip space.
       We do not perform any transformations, so we just set w=1.0 to indicate a point. */
    gl_Position = vec4(_position, 1.0);

    /* To minimize shader workload we utilize hardware interpolation to generate the ray direction in world space per fragment. */
    viewDir = mat3(uCameraRotation) * vec3(_position.x * uAspectRatio, _position.y, uFocalLength);
}