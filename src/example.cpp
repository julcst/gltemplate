#include "example.hpp"

#include "framework/gl/program.hpp"
#include "framework/gl/mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>
#include <iostream>

using namespace glm;

ExampleApp::ExampleApp() : App(800, 600) {
    std::vector<float> vertices = {
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3
    };
    mesh.load(vertices, indices);
    program.load("example.vert", "example.frag");
    lRes = program.uniform("uRes");
    lT = program.uniform("uT");
}

void ExampleApp::init() {}

void ExampleApp::render() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    program.bind();
    program.set(lRes, resolution);
    program.set(lT, time);
    mesh.draw();
}

void ExampleApp::keyCallback(int key, int action) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) close();
}

void ExampleApp::buildImGui() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("Read me!");
    ImGui::Button("Click me!");
    ImGui::End();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("%.1f FPS", 1.f / delta);
    ImGui::Text("%.0fx%.0f", resolution.x, resolution.y);
    ImGui::End();
}