#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include "framework/app.hpp"

class MainApp : public App {
   public:
    MainApp();

   protected:
    // void init() override;
    // void render() override;
    // void buildImGui() override;
    void keyCallback(Key key, Action action) override;
    // void clickCallback(Button button, Action action, const vec2& position) override;
    // void scrollCallback(float amount) override;
    // void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override;
    // void resizeCallback(const vec2& resolution) override;
};