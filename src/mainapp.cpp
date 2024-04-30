#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace glm;

#include "config.hpp"

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Config::PROJECT_NAME);
    // Allocate resources
}

void MainApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) App::close();
}