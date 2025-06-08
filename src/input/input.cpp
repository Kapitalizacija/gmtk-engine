#include "input.hpp"

namespace Sierra::Input {
    //Base definitions
    bool Input::mKeys[GLFW_KEY_LAST] = {};
    bool Input::mMouseButtons[GLFW_MOUSE_BUTTON_LAST] = {};
    double Input::mMouseX = 0.0;
    double Input::mMouseY = 0.0;
    glm::ivec2 Input::mMousePos = glm::ivec2(0, 0);
    double Input::mScrollOffset = 0.0;
    
    void Input::init(Window *window) {
        GLFWwindow *ref = window->get_glfw_window();
        
        glfwSetKeyCallback(ref, keyCallback);
        glfwSetMouseButtonCallback(ref, mouseButtonCallback);
        glfwSetCursorPosCallback(ref, cursorPositionCallback);
        glfwSetScrollCallback(ref, scrollCallback);
    
        LOG("Input system initialized");
    }

    void Input::keyCallback(GLFWwindow*, int key, int, int action, int) {
        if (key >= 0 && key < GLFW_KEY_LAST)
            mKeys[key] = action != GLFW_RELEASE;
    }

    void Input::mouseButtonCallback(GLFWwindow*, int button, int action, int) {
        if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
            mMouseButtons[button] = action != GLFW_RELEASE;
    }

    void Input::cursorPositionCallback(GLFWwindow*, double xpos, double ypos) {
        mMousePos = glm::ivec2(xpos, ypos);
    }

    void Input::scrollCallback(GLFWwindow*, double, double yoffset) {
        mScrollOffset = yoffset;
    }

    bool Input::isKeyPressed(int key) {
        return key >= 0 && key < GLFW_KEY_LAST && mKeys[key];
    }

    bool Input::isMouseButtonPressed(int button) {
        return button >= 0 && button < GLFW_MOUSE_BUTTON_LAST && mMouseButtons[button];
    }

    glm::ivec2 Input::getMousePosition() {
        return mMousePos;
    }

    double Input::getScrollOffset() {
        return mScrollOffset;
    }

    void Input::endFrame() {
        mScrollOffset = 0.0;
    }
}