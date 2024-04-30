#include "app.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <cassert>
#include <iostream>
#include <string>

using namespace glm;

App::App(unsigned int width, unsigned int height) : resolution(width, height), time(0.f), delta(0.f), frames(0), imguiEnabled(true) {
    initGLFW();
    initImGui();
    initGL();
}

void App::initGLFW() {
    // Init GLFW
    int glfwInitStatus = glfwInit();
    assert(glfwInitStatus);

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW] Error: " << description << std::endl;
    });

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE); // Enables SRGB rendering
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(resolution.x, resolution.y, "", NULL, NULL);
    assert(window);
    glfwSetWindowUserPointer(window, this);
    // Measure real resolution
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    resolution.x = w;
    resolution.y = h;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    mouse = vec2(x, y);
    // Callbacks
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->resolution.x = width;
        app->resolution.y = height;
        app->resizeCallback(app->resolution);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->keyCallback(static_cast<Key>(key), static_cast<Action>(action));
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        app->mouse.x = x;
        app->mouse.y = y;
        app->clickCallback(static_cast<Button>(button), static_cast<Action>(action), vec2(x, y));
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
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
    glfwSetScrollCallback(window, [](GLFWwindow* window, double, double yoffset) {
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->scrollCallback(static_cast<float>(yoffset));
    });
    glfwMakeContextCurrent(window);
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

const std::string glSourceToString(GLenum source) {
    switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION: return "Application";
        case GL_DEBUG_SOURCE_OTHER: return "Other";
        default: return "Unknown";
    }
}

const std::string glTypeToString(GLenum type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
        case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
        case GL_DEBUG_TYPE_MARKER: return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
        case GL_DEBUG_TYPE_OTHER: return "Other";
        default: return "Unknown";
    }
}

const std::string glSeverityToString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: return "High";
        case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
        case GL_DEBUG_SEVERITY_LOW: return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        default: return "Unknown";
    }
}

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "[" << glSourceToString(source) << "]" << glTypeToString(type) << ": " << message << "(Severity: " << glSeverityToString(severity) << ")" << std::endl;
}

const std::string glErrorToString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR: return "No error";
        case GL_INVALID_ENUM: return "Invalid enum";
        case GL_INVALID_VALUE: return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW: return "Stack overflow";
        case GL_STACK_UNDERFLOW: return "Stack underflow";
        case GL_OUT_OF_MEMORY: return "Out of memory";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
        case GL_CONTEXT_LOST: return "Context lost";
        default: return "Unknown error";
    
    }
}

void App::collectGLErrors() {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "[OpenGL] Error: " << glErrorToString(err) << std::endl;
    }
}

void App::initGL() {
    int gladLoadGLStatus = gladLoadGL();
    assert(gladLoadGLStatus);
    glEnable(GL_FRAMEBUFFER_SRGB); // Enables SRGB rendering
    // glEnable(GL_DEBUG_OUTPUT); // Enables debug output
    // glDebugMessageCallback(debugCallback, 0); // Only supported for OpenGL 4.3+
}

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

// To be overriden
void App::init() {}
void App::render() {}
void App::keyCallback(Key key, Action action) {}
void App::clickCallback(Button button, Action action, const vec2& position) {}
void App::scrollCallback(float amount) {}
void App::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {}
void App::resizeCallback(const vec2& resolution) {}
void App::buildImGui() {}

void App::run() {
    init();
    resizeCallback(resolution);
    frames = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        float current = static_cast<float>(glfwGetTime());
        delta = current - time;
        time = current;
        render();
        if (imguiEnabled) renderImGui();
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

void App::setTitle(const std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

void App::setVSync(bool vsync) {
    glfwSwapInterval(vsync ? 1 : 0);
}