#include "app.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <set>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
using namespace glm;

#include "framework/gl/texture.hpp"

App::App(unsigned int width, unsigned int height) : resolution(width, height) {
    initGLFW();
    initImGui();
    initGL();
}

void App::initGLFW() {
    // Init GLFW
    int glfwInitStatus = glfwInit();
    if (!glfwInitStatus) throw std::runtime_error("Failed to initialize GLFW");

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW] Error: " << description << std::endl;
    });

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifndef MODERN_GL
    // Stuck with OpenGL 4.1 for compatibility with macOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE); // Enables SRGB rendering
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    window = glfwCreateWindow(resolution.x, resolution.y, "", nullptr, nullptr);
    if (!window) throw std::runtime_error("Failed to create window");
    
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
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int modifier) {
        if (ImGui::GetIO().WantCaptureKeyboard) return;
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->keyCallback(static_cast<Key>(key), static_cast<Action>(action), static_cast<Modifier>(modifier));
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int modifier) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        app->mouse.x = x;
        app->mouse.y = y;
        app->clickCallback(static_cast<Button>(button), static_cast<Action>(action), static_cast<Modifier>(modifier));
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        if (ImGui::GetIO().WantCaptureMouse) return;
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
        if (ImGui::GetIO().WantCaptureMouse) return;
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->scrollCallback(static_cast<float>(xoffset), static_cast<float>(yoffset));
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

std::string glSourceToString(GLenum source) {
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

std::string glTypeToString(GLenum type) {
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

std::string glSeverityToString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: return "High";
        case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
        case GL_DEBUG_SEVERITY_LOW: return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
        default: return "Unknown";
    }
}

std::string glErrorToString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_CONTEXT_LOST: return "GL_CONTEXT_LOST";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
        default: return "Unknown";
    }
}

// Print function if error occurs
void gladPostCallback(const char* name, void* funcptr, int len_args, ...) {
    GLenum error = glad_glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << glErrorToString(error) << " in " << name << "(";
        va_list args;
        va_start(args, len_args);
        for (int i = 0; i < len_args; i++) {
            GLenum arg = va_arg(args, GLenum);
            std::cerr << arg;
            if (i < len_args - 1) std::cerr << ", ";
        }
        va_end(args);
        std::cerr << ")" << std::endl;
    }
}

void App::initGL() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) throw std::runtime_error("Failed to initialize GLAD");

    std::cout << std::endl
        << "OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl
        << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl
        << "GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
        << std::endl;

    glEnable(GL_FRAMEBUFFER_SRGB); // Enables SRGB rendering

    glad_set_post_callback(gladPostCallback);
    
    // Enables better debug output, only supported for OpenGL 4.3+
#ifdef MODERN_GL
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        auto app = static_cast<App*>(const_cast<void*>(userParam));
        if (app->seenMessageIDs.insert(id).second)
            std::cerr << "[" << glSourceToString(source) << "] " << glTypeToString(type) << ": " << message << " (" << glSeverityToString(severity) << ")" << std::endl;
    }, this);
#endif
}

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

// To be overriden
void App::render() {}
void App::keyCallback(Key key, Action action, Modifier modifier) {}
void App::clickCallback(Button button, Action action, Modifier modifier) {}
void App::scrollCallback(float xamount, float yamount) {}
void App::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {}
void App::resizeCallback(const vec2& resolution) {}
void App::buildImGui() {}

void App::run() {
    resizeCallback(resolution);
    frames = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        auto current = static_cast<float>(glfwGetTime());
        delta = current - time;
        time = current;
        render();
        if (imguiEnabled) renderImGui();
        glfwSwapBuffers(window); // Double Buffering
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

vec2 App::convertCursorToClipSpace() const {
    ivec2 windowSize;
    glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
    vec2 cursor = mouse / vec2(windowSize) * 2.0f - 1.0f;
    cursor.y = -cursor.y;
    return cursor;
}

bool App::takeScreenshot(const std::filesystem::path &path, GLenum baseFormat, GLenum attachment) {
#ifdef MODERN_GL
    glNamedFramebufferReadBuffer(0, attachment);
#else
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(attachment);
#endif

    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    GLenum dataType = GL_UNSIGNED_BYTE;
    GLint channels = getChannels(baseFormat);
    
    stbi_flip_vertically_on_write(true);

    auto ubyteData = std::make_unique<unsigned char[]>(width * height * channels);
    glReadPixels(0, 0, width, height, baseFormat, dataType, ubyteData.get());

    auto ext = path.extension();
    if (ext == ".png")
        return stbi_write_png(path.string().c_str(), width, height, channels, ubyteData.get(), width * channels);
    else if (ext == ".bmp")
        return stbi_write_bmp(path.string().c_str(), width, height, channels, ubyteData.get());
    else if (ext == ".tga")
        return stbi_write_tga(path.string().c_str(), width, height, channels, ubyteData.get());
    else if (ext == ".jpg" || ext == ".jpeg")
        return stbi_write_jpg(path.string().c_str(), width, height, channels, ubyteData.get(), 95);
    else
        throw std::runtime_error("Unsupported image format");
}

bool App::isKeyDown(Key key) const {
    return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
}