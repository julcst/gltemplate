#pragma once

#include "framework/app.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"

class ExampleApp : public App {
public:
    ExampleApp();
protected:
    void init();
    void buildImGui();
    void render();
    void keyCallback(int key, int action);
private:
    Mesh fullscreenTriangle;
    Program backgroundShader;
    GLuint lRes;
    GLuint lT;
    Mesh mesh;
    Program meshShader;
    GLuint lMVP;
};