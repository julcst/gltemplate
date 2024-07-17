#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

Camera::Camera() = default;

Camera::Camera(const vec3& worldPosition, const vec3& target, const vec3& up, float near, float far, float fov)
    : worldPosition(worldPosition), target(target), up(up), minDist(near), maxDist(far), fov(fov), near(near), far(far) {}

void Camera::orbit(const vec2& delta) {
    const vec3 relativePos = worldPosition - target;
    const vec3 direction = normalize(relativePos);
    const vec3 right = cross(direction, up); // Create a right vector orthogonal to camera direction and up
    const float maxUpDelta = acos(dot(direction, up)); // Maximum positive angle between camera direction and up
    const float maxDownDelta = -(pi<float>() - maxUpDelta); // Maximum negative angle between camera direction and down
    const quat rotation = rotate(angleAxis(-delta.x, up), clamp(delta.y, maxDownDelta + ALTITUDE_DELTA, maxUpDelta - ALTITUDE_DELTA), right);
    worldPosition = target + rotation * relativePos;
    invalidate();
}

void Camera::zoom(float delta) {
    vec3 direction = worldPosition - target;
    float distance = length(direction);
    direction /= distance;
    distance = clamp(distance - delta, minDist, maxDist);
    worldPosition = target + direction * distance;
    invalidate();
}

void Camera::moveInEyeSpace(const vec3& delta) {
    const vec3 camDelta = mat3(cameraMatrix) * delta; // Transform the delta to world space
    worldPosition += camDelta;
    target += camDelta;
    invalidate();
}

void Camera::resize(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    invalidate();
}

void Camera::invalidate() {
    isUpToDate = false;
}

mat4 calcViewMatrix(const vec3& worldPosition, const vec3& target, const vec3& up) {
    return lookAt(worldPosition, target, up);
}

mat4 calcProjectionMatrix(float fov, float aspectRatio, float near, float far) {
    return perspective(fov, aspectRatio, near, far);
}

float calcFocalLength(float fov) {
    const float tanHalfFovy = tan(fov / 2.0f);
    return 1.0f / tanHalfFovy;
}

void Camera::update() {
    viewMatrix = calcViewMatrix(worldPosition, target, up);
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