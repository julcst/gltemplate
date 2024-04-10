/**
 * The WorldBuffer uniform buffer binds at index 0 and stores information about the scene that is only uploaded once per frame
 */
layout(std140) uniform WorldBuffer {
// Location 0
    vec2 uResolution;
    float uTime;
    float uTimeDelta;
// Location 1
    float uAspectRatio;
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
    mat4 uLocalToClip;
// Location 4-7
    mat4 uLocalToWorld;
};