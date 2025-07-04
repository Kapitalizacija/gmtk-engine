#pragma once

#include <string>
#include <stdexcept>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "io/logging/logger.hpp"

namespace Sierra {
    struct Resolution {
        uint16_t w;
        uint16_t h;
    };
    
    class Window {
        public:
            Window(std::string name, Resolution resolution);
    
            Window(Window&& other) noexcept;
            void operator=(Window&& other) noexcept;
    
            Window(Window& other) = delete;
            void operator=(Window& other) = delete;
    
            ~Window();
    
            bool shouldClose();
            void update();

            Resolution get_resolution();
            GLFWwindow* get_glfw_window();
    
        private:
            void init_glfw();
            void create_window(std::string name);
            void init_glad();
    
            Resolution resolution;
    
            GLFWwindow* glfw_win;
    
    
            static size_t instance_count;
    };
}