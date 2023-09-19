#include "app.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>

#include <stdexcept>

using namespace glm;

App::App(int width, int height) : resolution(width, height), time(0.f), delta(0.f) {
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
    if(!glfwInit()) throw std::runtime_error("GLFW initialization failed");

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(resolution.x, resolution.y, "", NULL, NULL);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    resolution.x = width;
    resolution.y = height;
    if(!window) throw std::runtime_error("GLFW window creation failed");
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->keyCallback(key, action);
    });
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync
}

void App::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void App::initGL() {
    int version = gladLoadGL();
    if (!version) throw std::runtime_error("Failed to load OpenGL");
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
void App::keyCallback(int key, int action) {}
void App::buildImGui() {}

void App::run() {
    init();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        float current = (float) glfwGetTime();
        delta = current - time;
        time = current;
        render();
        if(imguiEnabled) renderImGui();
        glfwSwapBuffers(window);
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