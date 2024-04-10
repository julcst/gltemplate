#include "example.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>

#include "util.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"

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
    0, 1, 2, 2, 3, 0,  // Front
    1, 5, 6, 6, 2, 1,  // Right
    7, 6, 5, 5, 4, 7,  // Back
    4, 0, 3, 3, 7, 4,  // Left
    4, 5, 1, 1, 0, 4,  // Bottom
    3, 2, 6, 6, 7, 3,  // Top
};

ExampleApp::ExampleApp() : App(800, 600), cam(0.0f, 0.0f, 5.0f, 3.0f, 50.0f), worldUBO(0, world), objectUBO(1, object) {
    fullscreenTriangle.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);
    backgroundShader.load("screen.vert", "background.frag");
    backgroundShader.bindUBO("WorldBuffer", 0);
    backgroundShader.bindUBO("ObjectBuffer", 1);

    mesh.load(vertices, indices);
    meshShader.load("projection.vert", "voxels.frag");
    meshShader.bindUBO("WorldBuffer", 0);
    meshShader.bindUBO("ObjectBuffer", 1);
}

void ExampleApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void ExampleApp::render() {
    // Clear the screen
    glClear(GL_DEPTH_BUFFER_BIT);

    /////////////////////////////////////
    // Color pattern in the background //
    /////////////////////////////////////

    // Update per frame uniforms
    world.uResolution = resolution;
    world.uTime = time;
    world.uTimeDelta = delta;
    world.uAspectRatio = resolution.x / resolution.y;
    worldUBO.upload(world); // Send to GPU

    // Render
    glDepthMask(GL_FALSE);
    backgroundShader.bind();
    fullscreenTriangle.draw();

    ////////////////////////////
    // Mesh in the foreground //
    ////////////////////////////

    // Calculate transformations
    mat4 projMat = perspective(45.0f, resolution.x / resolution.y, 0.1f, 100.0f);
    mat4 viewMat = cam.calcView();
    mat4 modelMat = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));

    // Update per object uniforms
    object.uLocalToWorld = modelMat;
    object.uLocalToClip = projMat * viewMat * modelMat;
    objectUBO.upload(object); // Send to GPU

    // Render
    glDepthMask(GL_TRUE);
    meshShader.bind();
    mesh.draw();
}

void ExampleApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) close();
    // Toggle GUI with F1
    if (key == Key::COMMA && action == Action::PRESS) imguiEnabled = !imguiEnabled;
}

void ExampleApp::scrollCallback(float amount) {
    // Zoom camera with scroll wheel
    cam.zoom(amount);
}

void ExampleApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    // Rotate camera with right mouse button
    if (rightButton) cam.rotate(movement * 0.01f);
}

void ExampleApp::buildImGui() {
    ImGui::FPSWindow(delta, resolution);

    ImGui::Begin("Hello, world!");
    ImGui::Text("Read me!");
    ImGui::Button("Click me!");
    ImGui::End();
}