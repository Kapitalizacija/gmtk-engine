#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

class Instance {
    public:
        Instance(GLFWwindow* window);
        Instance(GLFWwindow* window, std::string app_name, uint32_t app_version);

        Instance(Instance& other) = delete;
        Instance(Instance&& other) = delete;

        ~Instance();

        VkInstance get_vk_instance();
    private:
        void create_vk_instance(std::string app_name, uint32_t app_version);
        std::vector<const char*> get_instance_extensions();

        VkInstance vk_instance;
};