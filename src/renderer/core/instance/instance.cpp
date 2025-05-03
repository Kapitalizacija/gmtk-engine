#include "instance.hpp"

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

std::vector<const char*> extensions = {
    
};

Instance::Instance(GLFWwindow* window) {
    create_vk_instance("placeholder_name", 0);    
}

Instance::Instance(GLFWwindow* window, std::string app_name, uint32_t app_version) {
    create_vk_instance(app_name, app_version);    
}

void Instance::create_vk_instance(std::string app_name, uint32_t app_version) {

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    app_info.pApplicationName = app_name.c_str(),
    app_info.applicationVersion = app_version;

    app_info.apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0);

    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "vrtacnik_engine";


    VkInstanceCreateInfo create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    create_info.pApplicationInfo = &app_info;

    std::vector<const char*> extensions = get_instance_extensions();

    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();


#ifdef DEBUG
    create_info.enabledLayerCount = validation_layers.size();
    create_info.ppEnabledLayerNames = validation_layers.data();
#endif


    if ( vkCreateInstance(&create_info, nullptr, &vk_instance) != VK_SUCCESS ) {
        throw new std::runtime_error("Failed to create a new Vulkan instance");
    }
}

std::vector<const char*> Instance::get_instance_extensions() {
    uint32_t extension_count;
    glfwGetRequiredInstanceExtensions(&extension_count);

    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    for ( int i = 0; i < extension_count; i++ ) {
        extensions.push_back(glfw_extensions[i]);
    }

    return extensions;
}

VkInstance Instance::get_vk_instance() {
    return vk_instance;
}

Instance::~Instance() {
    vkDestroyInstance(vk_instance, nullptr);
}