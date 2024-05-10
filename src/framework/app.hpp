#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

using namespace glm;

enum class Key {
    UNKNOWN = GLFW_KEY_UNKNOWN,
    SPACE = GLFW_KEY_SPACE,
    APOSTROPHE = GLFW_KEY_APOSTROPHE,
    COMMA = GLFW_KEY_COMMA, MINUS, PERIOD, SLASH,
    D0 = GLFW_KEY_0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
    SEMICOLON = GLFW_KEY_SEMICOLON, EQUAL = GLFW_KEY_EQUAL,
    A = GLFW_KEY_A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET,
    GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
    WORLD_1 = GLFW_KEY_WORLD_1, WORLD_2,
    ESC = GLFW_KEY_ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE, RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
    CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
    F1 = GLFW_KEY_F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
    KP_0 = GLFW_KEY_KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
    KP_DECIMAL = GLFW_KEY_KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
    LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU
};

enum class Button {
    LEFT = GLFW_MOUSE_BUTTON_LEFT, RIGHT, MIDDLE
};

enum class Action {
    RELEASE,
    PRESS,
    REPEAT
};

enum class Modifier {
    SHIFT = GLFW_MOD_SHIFT,
    CTRL = GLFW_MOD_CONTROL,
    ALT = GLFW_MOD_ALT,
    /* Windows or Command key*/
    SUPER = GLFW_MOD_SUPER,
    CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
    NUM_LOCK = GLFW_MOD_NUM_LOCK
};
inline bool operator>=(Modifier a, Modifier b) {
    return static_cast<int>(a) & static_cast<int>(b);
}
inline Modifier operator|(Modifier a, Modifier b) {
    return static_cast<Modifier>(static_cast<int>(a) | static_cast<int>(b));
}

class App {
   public:
    bool imguiEnabled;
    vec2 resolution;
    float time;
    float delta;
    unsigned int frames;
    vec2 mouse;
    GLFWwindow* window;

    App(unsigned int width, unsigned int height);
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&& other) = delete;
    App& operator=(App&& other) = delete;
    virtual ~App();
    void run();
    void close();
    void collectGLErrors();
    void setTitle(const std::string& title);
    void setVSync(bool vsync);

   protected:
    // To be overriden
    virtual void init();
    virtual void buildImGui();
    virtual void render();
    virtual void keyCallback(Key key, Action action);
    virtual void clickCallback(Button button, Action action, Modifier modifier);
    virtual void scrollCallback(float amount);
    virtual void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton);
    virtual void resizeCallback(const vec2& resolution);

   private:
    void initGLFW();
    void initImGui();
    void initGL();
    void renderImGui();
};