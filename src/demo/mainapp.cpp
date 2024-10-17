#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>
#include <iostream>

#include "framework/imguiutil.hpp"
#include "framework/app.hpp"
#include "framework/camera.hpp"
#include "framework/mesh.hpp"
#include "framework/uniformbuffer.hpp"
#include "framework/gl/program.hpp"
#include "framework/gl/texture.hpp"
#include "framework/gl/framebuffer.hpp"

using namespace glm;

MainApp::MainApp() : App(800, 600), worldUBO(0, world), objectUBO(1, object) {
    App::setVSync(true); // Enable vertical synchronization
    /* The background is rendered using a triangle that spans the whole frame */
    fullscreenTriangle.load(Mesh::FULLSCREEN_VERTICES, Mesh::FULLSCREEN_INDICES);
    backgroundShader.load("shaders/raygen.vert", "shaders/background.frag");
    backgroundShader.bindUBO("WorldBuffer", 0);
    backgroundShader.bindUBO("ObjectBuffer", 1);
    backgroundShader.bindTextureUnit("tCubemap", 0);

    mesh.loadWithTangents("meshes/bunny.obj");
    meshShader.load("shaders/projection.vert", "shaders/debug.frag");
    meshShader.bindUBO("WorldBuffer", 0);
    meshShader.bindUBO("ObjectBuffer", 1);
    meshShader.bindTextureUnit("tDiffuse", 0);

    traceOpenGLCalls = true; // Enable OpenGL call tracing

    texture.load(GL_SRGB8, "textures/checkerbw.png");
    texture.bindTextureUnit(0);

    cubemap.loadCubemap(GL_RGB16F, "textures/studio");
    cubemap.bindTextureUnit(0);
}

void MainApp::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void MainApp::render() {
    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    /* Update uniforms that only change once per frame */
    world.uResolution = resolution;
    world.uTime = time;
    world.uTimeDelta = delta;

    // Update camera
    const float camDelta = 5.0f * delta;
    if (isKeyDown(Key::W)) cam.moveInEyeSpace(vec3(0.0f, 0.0f, -camDelta));
    if (isKeyDown(Key::S)) cam.moveInEyeSpace(vec3(0.0f, 0.0f, camDelta));
    if (isKeyDown(Key::A)) cam.moveInEyeSpace(vec3(-camDelta, 0.0f, 0.0f));
    if (isKeyDown(Key::D)) cam.moveInEyeSpace(vec3(camDelta, 0.0f, 0.0f));
    if (isKeyDown(Key::Q)) cam.moveInEyeSpace(vec3(0.0f, camDelta, 0.0f));
    if (isKeyDown(Key::E)) cam.moveInEyeSpace(vec3(0.0f, -camDelta, 0.0f));

    if (cam.updateIfChanged()) {
        world.uAspectRatio = cam.aspectRatio;
        world.uCameraMatrix = cam.cameraMatrix;
        world.uFocalLength = cam.focalLength;
    }
    worldUBO.upload(world); // Send to GPU

    /* Render procedural sky in the background */
    glDepthMask(GL_FALSE); // Disable writing to the depth buffer
    backgroundShader.use(); // Bind shader
    fullscreenTriangle.draw(); // Draw fullscreen

    /* Calculate object transformation */
    mat4 projMat = cam.projectionMatrix;
    mat4 viewMat = cam.viewMatrix;
    mat4 modelMat = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));

    /* Update object specific uniforms */
    object.uLocalToWorld = modelMat;
    object.uLocalToClip = projMat * viewMat * modelMat;
    objectUBO.upload(object); // Send to GPU

    /* Render mesh with texture in the foreground */
    glDepthMask(GL_TRUE); // Enable writing to the depth buffer
    meshShader.use(); // Bind shader
    mesh.draw(); // Draw mesh
    traceOpenGLCalls = false; // Disable OpenGL call tracing
}

/* Catch window events by overriding the callback functions */

void MainApp::keyCallback(Key key, Action action, Modifier modifier) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) close();
    // Toggle GUI with COMMA
    if (key == Key::COMMA && action == Action::PRESS) imguiEnabled = !imguiEnabled;
    // Retarget the origin with R
    if (key == Key::R && action == Action::PRESS) {
        cam.target = vec3(0.0f);
        cam.invalidate();
    }
    // Take a screenshot with Shift + S
    if (key == Key::S && modifier >= Modifier::SHIFT && action == Action::PRESS) takeScreenshot("screenshot.bmp");
}

void MainApp::scrollCallback(float amount) {
    // Zoom camera with scroll wheel
    cam.zoom(amount);
}

void MainApp::moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) {
    // Rotate camera with right mouse button
    if (leftButton | rightButton | middleButton) cam.orbit(movement * 0.01f);
}

void MainApp::resizeCallback(const vec2& resolution) {
    // Resize camera with window
    cam.resize(resolution.x / resolution.y);
}

/* Build GUI elements here using the functions from imgui.h and util.cpp */
void MainApp::buildImGui() {
    /* Render FPS, frametime and resolution. FPS and frametime are rolling averages */
    ImGui::StatisticsWindow(delta, resolution);

    /* Render a simple window with text and a button */
    ImGui::Begin("Hello, world!");
    ImGui::Text("Read me!");
    ImGui::Button("Click me!");
    ImGui::Text("Press ESC to exit or COMMA to toggle GUI.");
    ImGui::End();
}