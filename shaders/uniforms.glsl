/**
 * Uniform buffers are used to pass information from the CPU to the GPU.
 * One has to be careful with the layout of the data in the uniform buffer to match the layout of the data inside the C++ code.
 * Each block in the uniform buffer is aligned at multiples of vec4 (4 floats, 16 bytes), therefore in the C++ code one sometimes has to insert padding bytes.
 * For further information see https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout.
 */

/**
 * The WorldBuffer uniform buffer binds at index 0 and stores information about the scene that is only uploaded once per frame
 */
layout(std140) uniform WorldBuffer {
// Location 0
    /* Resolution of the screen */
    vec2 uResolution;
    /* Time in seconds since the start of the program */
    float uTime;
    /* Time in seconds since the last frame */
    float uTimeDelta;
// Location 1
    /* Aspect ratio of the screen */
    float uAspectRatio;
    /* Focal length of the camera */
    float uFocalLength;
    // vec2 padding0;
// Location 2-5
    mat4 uCameraRotation;
};

/**
 * The ObjectBuffer uniform buffer binds at index 1 and stores information about the current object being rendered
 * and is uploaded once per object
 */
layout(std140) uniform ObjectBuffer {
// Location 0-3
    /* Model-View-Projection matrix of the object */
    mat4 uLocalToClip;
// Location 4-7
    /* Model matrix of the object */
    mat4 uLocalToWorld;
};