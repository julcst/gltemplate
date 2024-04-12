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
    void setTarget(const vec3& target);
    void setUp(const vec3& up);
    void setZoomRange(float minDist, float maxDist);
    void setFOV(float fov);

    mat4 calcViewMatrix();
    mat3 calcRotationMatrix();
    vec3 getCartesianPosition();
    mat4 calcProjectionMatrix(float aspectRatio, float near, float far);
    float calcFocalLength();

    bool hasChanged();

   private:
    bool changed;
    bool isCartesianPositionUpToDate;
    /* Relative position in spherical coordinates: (distance, azimuth, altitude) */
    vec3 sphericalPosition;
    vec3 cartesianPosition;
    vec3 target;
    vec3 up;
    float minDist;
    float maxDist;
    float fov;

    vec3 calcCartesianPosition();
};