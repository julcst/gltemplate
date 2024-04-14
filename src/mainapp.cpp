#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>

#include "util.hpp"
#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/gl/mesh.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/texture.hpp"
#include "framework/gl/uniformbuffer.hpp"

using namespace glm;

MainApp::MainApp() : App(800, 600), worldUBO(0, world), objectUBO(1, object) {
    /* The background is rendered using a triangle that spans the whole frame */
    fullscreenTriangle.load(FULLSCREEN_VERTICES, FULLSCREEN_INDICES);
    backgroundShader.load("screen.vert", "background.frag");
    backgroundShader.bindUBO("WorldBuffer", 0);
    backgroundShader.bindUBO("ObjectBuffer", 1);

    mesh.loadWithTangents("meshes/bunny.obj");
    meshShader.load("projection.vert", "debug.frag");
    meshShader.bindUBO("WorldBuffer", 0);
    meshShader.bindUBO("ObjectBuffer", 1);
    meshShader.bindTextureUnit("tDiffuse", 0);

    texture.load(Texture::Format::COLOR8, "textures/test.png", 5);
}

void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void MainApp::render() {
    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    /////////////////////////////////////
    // Color pattern in the background //
    /////////////////////////////////////

    // Update per frame uniforms
    world.uResolution = resolution;
    world.uTime = time;
    world.uTimeDelta = delta;
    world.uAspectRatio = resolution.x / resolution.y;
    if (cam.hasChanged()) {
        world.uCameraRotation = cam.calcRotationMatrix();
        world.uFocalLength = cam.calcFocalLength();
    }
    worldUBO.upload(world); // Send to GPU

    // Render
    glDepthMask(GL_FALSE);
    backgroundShader.bind();
    fullscreenTriangle.draw();

    ////////////////////////////
    // Mesh in the foreground //
    ////////////////////////////

    // Calculate transformations
    mat4 projMat = cam.calcProjectionMatrix(world.uAspectRatio, 0.01f, 100.0f);
    mat4 viewMat = cam.calcViewMatrix();
    mat4 modelMat = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));

    // Update per object uniforms
    object.uLocalToWorld = modelMat;
    object.uLocalToClip = projMat * viewMat * modelMat;
    objectUBO.upload(object); // Send to GPU

    // Render
    glDepthMask(GL_TRUE);
    texture.bind(Texture::Type::TEX2D, 0);
    meshShader.bind();
    mesh.draw();
}

/* Catch window events by overriding the callback functions */

void MainApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) close();
    // Toggle GUI with COMMA
    if (key == Key::COMMA && action == Action::PRESS) imguiEnabled = !imguiEnabled;
}

void MainApp::scrollCallback(float amount) {
    // Zoom camera with scroll wheel
    cam.zoom(amount);
}

void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    // Rotate camera with right mouse button
    if (rightButton) cam.rotate(movement * 0.01f);
}

void MainApp::buildImGui() {
    ImGui::StatisticsWindow(delta, resolution);

    ImGui::Begin("Hello, world!");
    ImGui::Text("Read me!");
    ImGui::Button("Click me!");
    ImGui::Text("Press ESC to exit or COMMA to toggle GUI.");
    ImGui::End();
}