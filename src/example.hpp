#pragma once

#include "framework/app.hpp"
#include "framework/shader.hpp"
#include "framework/vao.hpp"

class ExampleApp : public App {
protected:
    void init();
    void buildImGui();
    void render();
    void keyCallback(int key, int action);
private:
    VAO vao;
    ShaderProgram program;
};