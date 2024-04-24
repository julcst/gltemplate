#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <vector>
#include <iostream>

#include "config.hpp"

using namespace glm;

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    // Allocate resources
}

void MainApp::init() {
    // Initialize
}

void MainApp::render() {
    // Render
}

void MainApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) App::close();
}