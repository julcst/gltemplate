#include "app.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>

#include <cassert>

using namespace glm;

App::App(int width, int height) : resolution(width, height), time(0.f), delta(0.f), frames(0), imguiEnabled(true) {
    initGLFW();
    initImGui();
    initGL();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    app->resolution.x = width;
    app->resolution.y = height;
}

void App::initGLFW() {
    // Init GLFW
    int glfwInitStatus = glfwInit();
    assert(glfwInitStatus);

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(resolution.x, resolution.y, "", NULL, NULL);
    assert(window);
    glfwSetWindowUserPointer(window, this);
    // Measure real resolution
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    resolution.x = width;
    resolution.y = height;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    mouse = vec2(x, y);
    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->keyCallback(static_cast<Key>(key), static_cast<Action>(action));
    });
    glfwSetMouseButtonCallback(window, [] (GLFWwindow* window, int button, int action, int mods) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        app->mouse.x = x;
        app->mouse.y = y;
        app->clickCallback(static_cast<Button>(button), static_cast<Action>(action), vec2(x, y));
    });
    glfwSetCursorPosCallback(window, [] (GLFWwindow* window, double xpos, double ypos) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        vec2 movement = vec2(x - app->mouse.x, y - app->mouse.y);
        app->mouse.x = x;
        app->mouse.y = y;
        bool leftButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool rightButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        bool middleButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
        app->moveCallback(movement, leftButton, rightButton, middleButton);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->scrollCallback(static_cast<float>(yoffset));
    });
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync
}

void App::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void App::initGL() {
    int gladLoadGLStatus = gladLoadGL();
    assert(gladLoadGLStatus);
}

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if(window) glfwDestroyWindow(window);
    glfwTerminate();
}

// To be overriden
void App::init() {}
void App::render() {}
void App::keyCallback(Key key, Action action) {}
void App::clickCallback(Button button, Action action, vec2 position) {}
void App::scrollCallback(float amount) {}
void App::moveCallback(vec2 movement, bool leftButton, bool rightButton, bool middleButton) {}
void App::buildImGui() {}

void App::run() {
    init();
    frames = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        float current = (float) glfwGetTime();
        delta = current - time;
        time = current;
        render();
        if(imguiEnabled) renderImGui();
        glfwSwapBuffers(window);
        frames++;
    }
}

void App::renderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    buildImGui();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::close() {
    glfwSetWindowShouldClose(window, true);
}