#pragma once

#include <glm/glm.hpp>

using namespace glm;

const float ALTITUDE_DELTA = 0.1f;

class Camera {
   public:
    Camera();
    Camera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float minDist, float maxDist, float fov);

    void rotate(const vec2& delta);
    void zoom(float delta);
    void resize(float apsectRatio);

    void invalidate();
    void update();
    void updateIfChanged();
    bool hasChanged();

    /* Relative position in spherical coordinates: (distance, azimuth, altitude) */
    vec3 sphericalPosition = vec3(5.0f, 0.0f, 0.0f);
    vec3 target = vec3(0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    float minDist = 0.1f;
    float maxDist = 100.0f;
    float fov = radians(60.0f);
    float near = 0.1f;
    float far = 100.0f;
    float aspectRatio = 1.0f;

    /* Memebers recalculated on change */
    vec3 cartesianPosition;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 rotationMatrix;
    float focalLength;

  private:
    bool hasRecentlyChanged = false;
    bool isUpToDate = false;
};