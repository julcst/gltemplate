#pragma once

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"

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
    GLuint lRes;
    GLuint lT;
    Mesh mesh;
    Program meshShader;
    GLuint lMVP;
};