#include "example.hpp"

#include "framework/gl/program.hpp"
#include "framework/gl/mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <vector>

using namespace glm;

// Cube mesh
const std::vector<float> vertices = {
    -1.0f, -1.0f,  1.0f, // 0
     1.0f, -1.0f,  1.0f, // 1
     1.0f,  1.0f,  1.0f, // 2
    -1.0f,  1.0f,  1.0f, // 3
    -1.0f, -1.0f, -1.0f, // 4
     1.0f, -1.0f, -1.0f, // 5
     1.0f,  1.0f, -1.0f, // 6
    -1.0f,  1.0f, -1.0f, // 7
};
const std::vector<unsigned int> indices = {
    0, 1, 2, 2, 3, 0, // Front
    1, 5, 6, 6, 2, 1, // Right
    7, 6, 5, 5, 4, 7, // Back
    4, 0, 3, 3, 7, 4, // Left
    4, 5, 1, 1, 0, 4, // Bottom
    3, 2, 6, 6, 7, 3, // Top
};

ExampleApp::ExampleApp() : App(800, 600), cam(0.0f, 0.0f, 5.0f, 3.0f, 50.0f) {
    fullscreenTriangle.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);
    backgroundShader.load("screen.vert", "example.frag");
    lRes = backgroundShader.uniform("uRes");
    lT = backgroundShader.uniform("uT");

    mesh.load(vertices, indices);
    meshShader.load("projection.vert", "voxels.frag");
    lMVP = meshShader.uniform("uMVP");
}

void ExampleApp::init() {
    ImGui::GetStyle().ScaleAllSizes(1.2);
    ImGui::GetIO().FontGlobalScale = 1.2;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void ExampleApp::render() {
    glClear(GL_DEPTH_BUFFER_BIT);
    // Render color pattern in the background
    glDepthMask(GL_FALSE);
    backgroundShader.bind();
    backgroundShader.set(lRes, resolution);
    backgroundShader.set(lT, time);
    fullscreenTriangle.draw();
    // Render the mesh in the foreground
    mat4 projMat = perspective(45.0f, resolution.x / resolution.y, 0.1f, 100.0f);
    mat4 viewMat = cam.calcView();
    mat4 modelMat = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));
    glDepthMask(GL_TRUE);
    meshShader.bind();
    meshShader.set(lMVP, projMat * viewMat * modelMat);
    mesh.draw();
}

void ExampleApp::keyCallback(Key key, Action action) {
    if(key == Key::ESC && action == Action::PRESS) close();
}

void ExampleApp::scrollCallback(float amount) {
    cam.zoom(amount);
}

void ExampleApp::moveCallback(vec2 movement, bool leftButton, bool rightButton, bool middleButton) {
    if(rightButton) cam.rotate(movement * 0.01f);
}

void ExampleApp::buildImGui() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("Read me!");
    ImGui::Button("Click me!");
    ImGui::End();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("%2.1ffps avg: %2.1ffps %.0fx%.0f", 1.f / delta, ImGui::GetIO().Framerate, resolution.x, resolution.y);
    ImGui::PopStyleColor();
    ImGui::End();
}