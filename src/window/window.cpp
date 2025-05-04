#include "window.hpp"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
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
    }
    
    void Window::init_glfw() {
        if ( glfwInit() == GLFW_FALSE ) {
            throw new std::runtime_error("Failed to init GLFW");
        }
    
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    
    void Window::init_glad() {
        if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
            throw std::runtime_error("Failed to init GLAD, womp womp");
        }

        glViewport(0, 0, resolution.w, resolution.h);
//        glEnable(GL_DEPTH_TEST);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }

    bool Window::should_close() {
        return glfwWindowShouldClose(glfw_win);
    }

    void Window::update() {
        glfwSwapBuffers(glfw_win);
        glfwPollEvents();
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
}
