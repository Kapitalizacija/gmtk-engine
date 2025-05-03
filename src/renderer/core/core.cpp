#include "core.hpp"

Core::Core(Window& window):
 instance(window.get_glfw_window()),
 device(instance.get_vk_instance()),
 swapchain(
    {
        window.get_glfw_window(),
        instance.get_vk_instance(),
        device,
        3      
    }) 
    {

}

Core::~Core() {

}