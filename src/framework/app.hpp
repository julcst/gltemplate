#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include <glbinding/gl46core/gl.h>
using namespace gl46core;

#include <string>
#include <filesystem>
#include <set>

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

/**
 * @brief Check if a modifier is part of a combination of modifiers.
 * SHIFT >= SHIFT | CTRL | ALT -> true
 * SHIFT >= CTRL | ALT -> false
 */
inline bool operator>=(Modifier a, Modifier b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

/**
 * @brief Combines modifiers
 */
inline Modifier operator|(Modifier a, Modifier b) {
    return static_cast<Modifier>(static_cast<int>(a) | static_cast<int>(b));
}

class App {
   public:
    /**
     * @brief Enable or disable the ImGui interface.
     */
    bool imguiEnabled = true;

    /**
     * @brief The width and height of the window in pixels.
     */
    vec2 resolution;

    /**
     * @brief Time in seconds since the start of the application.
     */
    float time = 0.0f;

    /**
     * @brief Time in seconds since the last frame.
     */
    float delta = 0.0f;

    /**
     * @brief The number of frames since the start of the application.
     */
    unsigned int frames = 0;

    /**
     * @brief Enable or disable logging of all OpenGL calls.
     */
    bool traceOpenGLCalls = false;

    /**
     * @brief Set of OpenGL message IDs that have already been seen.
     */
    std::set<GLuint> seenMessageIDs;

    /**
     * @brief The position of the mouse cursor in screen coordinates.
     * Can be converted to clip space with `convertCursorToClipSpace()`.
     */
    vec2 mouse;

    /**
     * @brief Identifier of the GLFW window.
     */
    GLFWwindow* window;

    /**
     * @brief Constructs an App object with the specified width and height.
     * Initializes GLFW and OpenGL.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     */
    App(unsigned int width, unsigned int height);

    /**
     * @brief Copy constructor (deleted).
     */
    App(const App&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     */
    App& operator=(const App&) = delete;

    /**
     * @brief Move constructor (deleted).
     */
    App(App&& other) = delete;

    /**
     * @brief Move assignment operator (deleted).
     */
    App& operator=(App&& other) = delete;

    /**
     * @brief Destructor, closes the GLFW window.
     */
    virtual ~App();

    /**
     * @brief Starts the render loop.
     */
    void run();

    /**
     * @brief Marks the window for closing.
     */
    void close();

    /**
     * @brief Sets the title of the window.
     * @param title The new title of the window.
     */
    void setTitle(const std::string& title);

    /**
     * @brief Enables or disables vertical synchronization.
     */
    void setVSync(bool vsync);

    /**
     * @brief Converts the cursor position to clip space.
     */
    vec2 convertCursorToClipSpace() const;

    /**
     * @brief Writes the color attachment of the default framebuffer to a file.
     * @param path The path to write the image to. Supported formats are PNG, BMP, TGA, and JPG. BMP and TGA are generally fastest because they are uncompressed.
     * @param baseFormat The base format of the image, e.g. `GL_RGBA`, `GL_RGB`, mainly used to enable/disable reading the alpha channel.
     * @param attachment The attachment point, e.g. `GL_FRONT`, `GL_BACK`, (Reading from specialized attachments like `GL_COLOR_ATTACHMENTi` and `GL_DEPTH_ATTACHMENT` is not supported for the default framebuffer).
     * By default `GL_BACK` as in double-buffered configurations reading from the front buffer is not advisable.
     */
    bool takeScreenshot(const std::filesystem::path& path, GLenum baseFormat = GL_RGB, GLenum attachment = GL_BACK);

    /**
     * @brief Checks if a key is currently pressed.
     */
    bool isKeyDown(Key key) const;

   protected:
    // To be overriden
    virtual void init();
    virtual void buildImGui();
    virtual void render();
    virtual void keyCallback(Key key, Action action, Modifier modifier);
    virtual void clickCallback(Button button, Action action, Modifier modifier);
    virtual void scrollCallback(float amount);
    virtual void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton);
    virtual void resizeCallback(const vec2& resolution);

   private:
    void initGLFW();
    void initImGui();
    void initGL();
    void renderImGui();
    void registerGLLoggingCallback();
};