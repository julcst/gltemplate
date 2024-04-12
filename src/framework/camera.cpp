#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera()
    : sphericalPosition(vec3(5.0f, 0.0f, 0.0f)), target(vec3(0.0f)), up(vec3(0.0f, 1.0f, 0.0f)), minDist(0.1f), maxDist(100.0f), fov(45.0f), changed(true), isCartesianPositionUpToDate(false) {}

Camera::Camera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float minDist, float maxDist, float fov)
    : sphericalPosition(sphericalPosition), target(target), up(up), minDist(minDist), maxDist(maxDist), fov(fov), changed(true), isCartesianPositionUpToDate(false) {}

void Camera::rotate(const vec2& delta) {
    sphericalPosition += vec3(0.0f, -delta.x, delta.y);
    sphericalPosition.y = mod(sphericalPosition.y, two_pi<float>());
    sphericalPosition.z = clamp(sphericalPosition.z, -half_pi<float>() + ALTITUDE_DELTA, half_pi<float>() - ALTITUDE_DELTA);
    isCartesianPositionUpToDate = false;
    changed = true;
}

void Camera::zoom(float delta) {
    sphericalPosition.x -= delta;
    sphericalPosition.x = clamp(sphericalPosition.x, minDist, maxDist);
    isCartesianPositionUpToDate = false;
    changed = true;
}

void Camera::setTarget(const vec3& target) {
    Camera::target = target;
    isCartesianPositionUpToDate = false;
    changed = true;
}

void Camera::setUp(const vec3& up) {
    Camera::up = up;
    changed = true;
}

void Camera::setZoomRange(float minDist, float maxDist) {
    Camera::minDist = minDist;
    Camera::maxDist = maxDist;
    sphericalPosition.x = clamp(sphericalPosition.x, minDist, maxDist);
    isCartesianPositionUpToDate = false;
    changed = true;
}

void Camera::setFOV(float fov) {
    Camera::fov = fov;
    changed = true;
}

mat4 Camera::calcViewMatrix() {
    return lookAt(getCartesianPosition(), target, up);
}

mat3 Camera::calcRotationMatrix() {
    vec3 forward = normalize(target - getCartesianPosition());
    vec3 right = normalize(cross(forward, up));
    vec3 newUp = cross(right, forward);
    return mat3(right, newUp, forward);
}

vec3 Camera::calcCartesianPosition() {
    const float azimuth = sphericalPosition.y;
    const float altitude = sphericalPosition.z;
    const float radius = sphericalPosition.x;
    return vec3(cos(altitude) * sin(azimuth), sin(altitude), cos(altitude) * cos(azimuth)) * radius + target;
}

vec3 Camera::getCartesianPosition() {
    if (!isCartesianPositionUpToDate) {
        cartesianPosition = calcCartesianPosition();
        isCartesianPositionUpToDate = true;
    }
    return cartesianPosition;
}

mat4 Camera::calcProjectionMatrix(float aspectRatio, float near, float far) {
    return perspective(fov, aspectRatio, near, far);
}

float Camera::calcFocalLength() {
    const float tanHalfFovy = tan(fov / 2.0f);
    return 1.0f / tanHalfFovy;
}

bool Camera::hasChanged() {
    bool hasChanged = changed;
    changed = false;
    return hasChanged;
}