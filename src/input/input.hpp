#pragma once

#include <GLFW/glfw3.h>
#include "io/logging/logger.hpp"

namespace Sierra::Input {
    class Input {
        public:
            static void init(GLFWwindow *window);
        
            static bool isKeyPressed(int key);
            static bool isMouseButtonPressed(int button);
            static void getMousePosition(double &x, double &y);
            static double getScrollOffset();
            static void endFrame();
        
            static void keyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
            static void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
            static void cursorPositionCallback(GLFWwindow*, double xpos, double ypos);
            static void scrollCallback(GLFWwindow*, double xoffset, double yoffset);
        
        private:
            static bool mKeys[GLFW_KEY_LAST];
            static bool mMouseButtons[GLFW_MOUSE_BUTTON_LAST];
            static double mMouseX, mMouseY;
            static double mScrollOffset;
    };
}