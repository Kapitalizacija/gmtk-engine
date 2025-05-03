#pragma once

#include <string>
#include <stdexcept>

#include <GLFW/glfw3.h>

struct Resolution {
    uint16_t w;
    uint16_t h;
};

class Window {
    public:
        Window(std::string name, Resolution resolution);

        Window(Window&& other);
        void operator=(Window&& other);

        Window(Window& other) = delete;
        void operator=(Window& other) = delete;

        ~Window();


        Resolution get_resolution();
        GLFWwindow* get_glfw_window();

    private:
        void init_glfw();
        void create_window(std::string name);

        Resolution resolution;

        GLFWwindow* glfw_win;


        static size_t instance_count;
};