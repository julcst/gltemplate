#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class App {
public:
    void run();
protected:
    // To be overriden
    virtual void init();
    virtual void buildImGui();
    virtual void render();
    virtual void keyCallback(int key, int action);
    // To interact with the window
    void close();
private:
    bool imguiEnabled = true;
    GLFWwindow* window;
    void initGLFW();
    void initImGui();
    void initGL();
    void renderImGui();
    void error(int error, const char* description);
    void cleanup();
};