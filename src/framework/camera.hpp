#pragma once

#include <glm/glm.hpp>

using namespace glm;

const float ALTITUDE_DELTA = 0.1f;

/**
 * @struct Camera
 * @brief Represents a camera orbiting around a target in spherical coordinates.
 */
struct Camera {
   public:
    Camera();
    Camera(const vec3& sphericalPosition, const vec3& target, const vec3& up, float minDist, float maxDist, float fov);

    /**
     * @brief Rotate the camera around the target.
     * @param delta The angles to rotate the camera in radians in the format (azimuth, altitude).
     */
    void rotate(const vec2& delta);

    /**
     * @brief Moves the camera in the direction of the target.
     * @param delta The distance to move the camera. Negative values move the camera away from the target.
     */
    void zoom(float delta);

    /**
     * @brief Resizes the camera with a new aspect ratio.
     * @note Call this if the window size changes.
     * @param aspectRatio The new aspect ratio of the camera = width / height.
     */
    void resize(float aspectRatio);

    /**
     * @brief Invalidates the camera, forcing an update on the next call to `update()`.
     */
    void invalidate();

    /**
     * @brief Updates the camera matrices and position.
     * Updates the view matrix, camera matrix, projection matrix, focal length, and world position.
     */
    void update();

    /**
     * @brief Updates the camera with `update()` if it has changed since the last update.
     * @return True if the camera has changed and was updated, false otherwise. Can be used to skip uniform updates.
     */
    bool updateIfChanged();

    /**
     * @brief Relative position in spherical coordinates: (distance, azimuth, altitude). Position is relative to the target, the camera orbits around the target.
     */
    vec3 sphericalPosition = vec3(5.0f, 0.0f, 0.0f);

    /**
     * @brief Target position in world coordinates. The target is where the camera looks at and orbits around.
     */
    vec3 target = vec3(0.0f);

    /**
     * @brief Up vector in world coordinates. The up vector defines the orientation of the camera. Default is the positive y-Axis.
     */
    vec3 up = vec3(0.0f, 1.0f, 0.0f);

    /**
     * @brief Minimal distance between camera and target. The camera will not get closer to the target than this distance when calling zoom.
     */
    float minDist = 0.1f;

    /**
     * @brief Maximal distance between camera and target. The camera will not get further away from the target than this distance when calling zoom.
     */
    float maxDist = 100.0f;

    /**
     * @brief Field of view in radians. The field of view defines the angle of the camera lens.
     */
    float fov = radians(60.0f);

    /**
     * @brief Near plane of the camera frustum. The near plane is the closest point to the camera that is still visible. Used to calculate the projection matrix.
     */
    float near = 0.1f;

    /**
     * @brief Far plane of the camera frustum. The far plane is the furthest point from the camera that is still visible. Used to calculate the projection matrix.
     */
    float far = 100.0f;

    /**
     * @brief Aspect ratio of the camera = width / height. Used to calculate the projection matrix.
     */
    float aspectRatio = 1.0f;

    /**
     * @brief The world space position of the camera.
     * @note This value is automatically calculated by `update()`.
     */
    vec3 worldPosition;

    /**
     * @brief The view matrix of the camera transforms world space to camera space.
     * @note This value is automatically calculated by `update()`.
     */
    mat4 viewMatrix;

    /**
     * @brief The camera matrix of the camera transforms camera space to world space and is thus the inverse of the view matrix.
     * @note This value is automatically calculated by `update()`.
     */
    mat4 cameraMatrix;

    /**
     * @brief The projection matrix of the camera transforms camera space to clip space.
     * @note This value is automatically calculated by `update()`.
     */
    mat4 projectionMatrix;

    /**
     * @brief The focal length is the distance is of the focal point of the camera lens.
     * @note This value is automatically calculated by `update()` from the field of view angle.
     */
    float focalLength;

  private:
    bool isUpToDate = false;
};