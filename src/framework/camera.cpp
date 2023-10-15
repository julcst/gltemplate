#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(float azimuth, float altitude, float dist, float minDist, float maxDist, vec3 target, vec3 up)
: sphericalPosition(dist, azimuth, altitude), minDist(minDist), maxDist(maxDist), target(target), up(up), changed(true) {}

void Camera::rotate(vec2 delta) {
    sphericalPosition += vec3(0.0f, -delta.x, delta.y);
    sphericalPosition.y = mod(sphericalPosition.y, two_pi<float>());
    sphericalPosition.z = clamp(sphericalPosition.z, -half_pi<float>() + ALTITUDE_DELTA, half_pi<float>() - ALTITUDE_DELTA);
    changed = true;
}

void Camera::zoom(float delta) {
    sphericalPosition.x -= delta;
    sphericalPosition.x = clamp(sphericalPosition.x, minDist, maxDist);
    changed = true;
}

mat4 Camera::calcView() {
    return lookAt(getPosition(), target, up);
}

mat3 Camera::calcRotation() {
    vec3 forward = normalize(target - getPosition());
    vec3 right = normalize(cross(forward, up));
    vec3 newUp = cross(right, forward);
    return mat3(right, newUp, forward);
}

vec3 Camera::calcPosition() {
    const float azimuth = sphericalPosition.y;
    const float altitude = sphericalPosition.z;
    const float radius = sphericalPosition.x;
    return vec3(cos(altitude) * sin(azimuth), sin(altitude), cos(altitude) * cos(azimuth)) * radius + target;
}

vec3 Camera::getPosition() {
    if(changed) {
        cartesianPosition = calcPosition();
        changed = false;
    }
    return cartesianPosition;
}