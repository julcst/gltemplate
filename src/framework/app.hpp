#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

using namespace glm;

class App {
public:
    vec2 resolution;
    float time;
    float delta;
    App(int width, int height);
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&& other) = delete;
    App& operator=(App&& other) = delete;
    virtual ~App();
    void run();
    void close();
protected:
    // To be overriden
    virtual void init();
    virtual void buildImGui();
    virtual void render();
    virtual void keyCallback(int key, int action);
private:
    bool imguiEnabled = true;
    GLFWwindow* window;
    void initGLFW();
    void initImGui();
    void initGL();
    void renderImGui();
};