#include "mainapp.hpp"

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
using namespace glm;
using namespace gl;

#include <framework/context.hpp>

MainApp::MainApp() : App(800, 600) {
    App::setTitle(Context::PROJECT_NAME);
    App::setVSync(true);
    // Allocate resources
}

void MainApp::keyCallback(Key key, Action action) {
    // Close the application when pressing ESC
    if (key == Key::ESC && action == Action::PRESS) App::close();
}