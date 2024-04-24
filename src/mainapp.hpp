#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    void init() override;
    void render() override;
    void keyCallback(Key key, Action action) override;
};