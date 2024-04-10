#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/uniformbuffer.hpp"

struct WorldBuffer {
// Location 0
    vec2 uResolution = vec2(0.0f);
    float uTime = 0.0f;
    float uTimeDelta = 0.0f;
// Location 1
    float uAspectRatio = 1.0f;
    float uFocalLength = 1.0f;
    vec2 padding0 = vec2(0.0f);
// Location 2-5
    mat4 uCameraRotation = mat4(1.0f);
};

struct ObjectBuffer {
// Location 0-3
    mat4 uLocalToClip = mat4(1.0f);
// Location 4-7
    mat4 uLocalToWorld = mat4(1.0f);
};

class ExampleApp : public App {
   public:
    ExampleApp();

   protected:
    void init() override;
    void buildImGui() override;
    void render() override;
    void keyCallback(Key key, Action action) override;
    void scrollCallback(float amount) override;
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;

   private:
    Camera cam;
    Mesh fullscreenTriangle;
    Program backgroundShader;
    Mesh mesh;
    Program meshShader;
    WorldBuffer world;
    ObjectBuffer object;
    UniformBuffer<WorldBuffer> worldUBO;
    UniformBuffer<ObjectBuffer> objectUBO;
};