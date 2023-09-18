#include "example.hpp"

#include "framework/shader.hpp"
#include "framework/vao.hpp"

#include <glad/glad.h>
#include <imgui.h>

void ExampleApp::init() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    vao = VAO(vertices, 4, indices, 2);
    program = ShaderProgram("example.vert", "example.frag");
    //lRes = program.uniform("uRes");
    //lT = program.uniform("uT");
}

void ExampleApp::render() {
    glClearColor(0.2f, 0.4f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    program.bind();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    ImGui::End();
}