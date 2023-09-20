#pragma once

#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"

class ExampleApp : public App {
public:
    ExampleApp();
protected:
    void init();
    void buildImGui();
    void render();
    void keyCallback(Key key, Action action);
    void scrollCallback(float amount);
    void moveCallback(vec2 movement, bool leftButton, bool rightButton, bool middleButton);
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