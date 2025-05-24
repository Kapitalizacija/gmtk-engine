#include "window.hpp"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ) {

    if (severity <= GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
           LOG(message); 
           break;
        case GL_DEBUG_SEVERITY_MEDIUM:
           WARN(message); 
           break;
        case GL_DEBUG_SEVERITY_HIGH:
           ERROR(message); 
           throw new std::runtime_error("problem izkusenj");
           break;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


// During init, enable debug output

namespace GMTKEngine {
    size_t Window::instance_count = 0;

    Window::Window(std::string name, Resolution resolution):
     resolution(resolution), glfw_win(nullptr) {
    
        if (instance_count == 0) {
            init_glfw();
        }
    
        create_window(name);
        init_glad();
    
        instance_count++;
    }
    
    void Window::create_window(std::string name) {
        glfw_win = glfwCreateWindow(resolution.w, resolution.h, name.c_str(), nullptr, nullptr);
    
        if ( glfw_win == nullptr ) {
            throw new std::runtime_error("Failed to create new GLFW window");
        }

        glfwMakeContextCurrent(glfw_win);

        glfwSetWindowSizeCallback(glfw_win, framebufferSizeCallback);
        glfwSwapInterval(2);
    }
    
    void Window::init_glfw() {
#ifdef __linux__
#ifdef DEBUG
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
#endif

        if ( glfwInit() == GLFW_FALSE ) {
            throw new std::runtime_error("Failed to init GLFW");
        }
    
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
    }
    
    void Window::init_glad() {
        if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
            throw std::runtime_error("Failed to init GLAD, womp womp");
        }

        glViewport(0, 0, resolution.w, resolution.h);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_DEBUG_OUTPUT);

#ifdef DEBUG
        glDebugMessageCallback(MessageCallback, 0);
#endif

    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(glfw_win);
    }

    void Window::update() {
        glfwSwapBuffers(glfw_win);
        glfwPollEvents();
    }
    
    GLFWwindow* Window::get_glfw_window() {
        return glfw_win;
    }
    
    Window::Window(Window&& other) noexcept{
        resolution = other.resolution;
        glfw_win = other.glfw_win;
    
        other.glfw_win = nullptr;
    }
    
    void Window::operator=(Window&& other) noexcept {
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
}
