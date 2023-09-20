#pragma once

#include <glm/glm.hpp>

using namespace glm;

const float ALTITUDE_DELTA = 0.1f;

class Camera {
public:
    Camera(float azimuth, float altitude, float dist, float minDist = 0.01f, float maxDist = 100.0f, vec3 target = vec3(0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));
    void rotate(vec2 delta);
    void zoom(float delta);
    mat4 calcView();
    mat3 calcRotation();
    vec3 getPosition();
private:
    vec3 calcPosition();
    bool changed;
    vec3 sphericalPosition;
    vec3 cartesianPosition;
    vec3 target;
    vec3 up;
    const float minDist;
    const float maxDist;
};