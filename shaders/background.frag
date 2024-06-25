#version 330 core

/* The input from the vertex shader. This has to be the same format as the output from the vertex shader */
in vec3 viewDir;
out vec3 fragColor;
uniform samplerCube tCubemap;

/* We outsource the definition of the uniforms to a separate file to avoid repetition. */
#include "uniforms.glsl"
/* The #line XX YYY macro is used for correct error messages, errors will be shown as "ERROR: YYY:??: ..." in terminal.
   This is necessary because the #include macro messes up our line numbering. Specify the number of the next line with XX and the file with YYY. */
#line 12 101

/**
 * Procedural sky background
 */
void main() {
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 sky = exp(-abs(rayDir.y) / vec3(0.1, 0.3, 0.6));
    // fragColor = sky; // Output to screen
    fragColor = texture(tCubemap, rayDir * vec3(-1, 1, 1)).rgb;
}