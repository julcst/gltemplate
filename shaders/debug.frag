#version 330 core

/* The input from the vertex shader. This has to be the same format as the output of the vertex shader */
in VertexData {
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
    vec3 worldTangent;
};
/* A texture sampler */
uniform sampler2D tDiffuse;
/* The output of the fragment shader, both vec4 and vec3 is possible but we currently do not care about alpha */
out vec3 fragColor;

/* Includes common functions and macros for debugging purposes */
#include "debug.glsl"
/* We outsource the definition of the uniforms to a separate file to avoid repetition. */
#include "uniforms.glsl"
/* The #line XX YYY macro is used for correct error messages, errors will be shown as "ERROR: YYY:??: ..." in terminal.
   This is necessary because the #include macro messes up our line numbering. Specify the number of the next line with XX and the file with YYY. */
#line 22 102

/**
 * Simpe test fragment shader
 */
void main() {
    /* The normal is interpolated by the rasterizer, so we need to manually renormalize it to ensure a unit vector */
    vec3 N = normalize(worldNormal); // Renormalize after interpolation
    fragColor = texture(tDiffuse, uv).rgb;
    /* Output the normal as an RGB color and visualize the UV-mapping */
    //fragColor = normalToRGB(N) * uvCheckerboard(uv, 100.0);
}