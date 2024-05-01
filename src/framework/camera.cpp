#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera() {}

Camera::Camera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float near, float far, float fov)
    : sphericalPosition(sphericalPosition), target(target), up(up), minDist(near), maxDist(far), fov(fov), near(near), far(far) {}

void Camera::rotate(const vec2& delta) {
    sphericalPosition += vec3(0.0f, -delta.x, delta.y);
    sphericalPosition.y = mod(sphericalPosition.y, two_pi<float>());
    sphericalPosition.z = clamp(sphericalPosition.z, -half_pi<float>() + ALTITUDE_DELTA, half_pi<float>() - ALTITUDE_DELTA);
    invalidate();
}

void Camera::zoom(float delta) {
    sphericalPosition.x -= delta;
    sphericalPosition.x = clamp(sphericalPosition.x, minDist, maxDist);
    invalidate();
}

void Camera::resize(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    invalidate();
}

void Camera::invalidate() {
    isUpToDate = false;
}

mat4 calcViewMatrix(const vec3& cartesianPosition, const vec3& target, const vec3& up) {
    return lookAt(cartesianPosition, target, up);
}

vec3 calcCartesianPosition(const vec3& sphericalPosition, const vec3& target) {
    const float azimuth = sphericalPosition.y;
    const float altitude = sphericalPosition.z;
    const float radius = sphericalPosition.x;
    return vec3(cos(altitude) * sin(azimuth), sin(altitude), cos(altitude) * cos(azimuth)) * radius + target;
}

mat4 calcProjectionMatrix(float fov, float aspectRatio, float near, float far) {
    return perspective(fov, aspectRatio, near, far);
}

float calcFocalLength(float fov) {
    const float tanHalfFovy = tan(fov / 2.0f);
    return 1.0f / tanHalfFovy;
}

void Camera::update() {
    cartesianPosition = calcCartesianPosition(sphericalPosition, target);
    viewMatrix = calcViewMatrix(cartesianPosition, target, up);
    // If we only care about the rotation, we can just transpose because rotation matrices are orthonormal
    // rotationMatrix = transpose(mat3(viewMatrix));
    cameraMatrix = inverse(viewMatrix);
    projectionMatrix = calcProjectionMatrix(fov, aspectRatio, near, far);
    focalLength = calcFocalLength(fov);
    isUpToDate = true;
}

bool Camera::updateIfChanged() {
    if (!isUpToDate) {
        update();
        return true;
    } else {
        return false;
    }
}