#include "window.hpp"

size_t Window::instance_count = 0;

Window::Window(std::string name, Resolution resolution):
 resolution(resolution), glfw_win(nullptr) {

    if (instance_count == 0) {
        init_glfw();
    }

    create_window(name);

    instance_count++;
}

void Window::create_window(std::string name) {
    glfw_win = glfwCreateWindow(resolution.w, resolution.h, name.c_str(), nullptr, nullptr);

    if ( glfw_win == nullptr ) {
        throw new std::runtime_error("Failed to create new GLFW window");
    }
}

void Window::init_glfw() {
    if ( glfwInit() == GLFW_FALSE ) {
        throw new std::runtime_error("Failed to init GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

GLFWwindow* Window::get_glfw_window() {
    return glfw_win;
}

Window::Window(Window&& other) {
    resolution = other.resolution;
    glfw_win = other.glfw_win;

    other.glfw_win = nullptr;
}

void Window::operator=(Window&& other) {
    resolution = other.resolution;
    glfw_win = other.glfw_win;

    other.glfw_win = nullptr;
}

Window::~Window() {
    if ( glfw_win == nullptr ) {
        return;
    }

    glfwDestroyWindow(glfw_win);

    if ( instance_count == 1 ) {
        glfwTerminate();
    }

    instance_count--;
}