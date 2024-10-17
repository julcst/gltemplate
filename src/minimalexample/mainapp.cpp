#include "mainapp.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace glm;

#include <framework/context.hpp>

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Context::PROJECT_NAME);
    App::setVSync(true);
    // Allocate resources
}

void MainApp::keyCallback(Key key, Action action, Modifier modifier) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) App::close();
}