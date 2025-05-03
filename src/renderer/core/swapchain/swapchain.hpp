#pragma once

#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../device/device.hpp"

struct SwapchainCreateInfo {
    GLFWwindow* glfw_window;
    VkInstance vk_instance;
    Device& device;

    uint32_t preferred_image_count;
};

struct SurfaceCapabilities {
    uint32_t image_count;
    VkExtent2D extent;
};

class Swapchain { 
    public:
        Swapchain(SwapchainCreateInfo& create_info);
        Swapchain(SwapchainCreateInfo&& create_info);

        Swapchain(Swapchain& other) = delete;
        Swapchain(Swapchain&& other) = delete;

        ~Swapchain();

        uint32_t get_swapchain_image_count();
        

    private:
        void create_surface(GLFWwindow* glfw_window, VkInstance vk_instance);
        void retrieve_surface_capabilities(VkPhysicalDevice vk_physical_device, uint32_t preferred_image_count);
        void pick_surface_format(VkPhysicalDevice vk_physical_device);
        void create_swapchain(Device& device);
        void retrieve_swapchain_images();

        VkSurfaceKHR vk_surface;
        VkSwapchainKHR vk_swapchain;

        SurfaceCapabilities capabilities;
        VkSurfaceFormatKHR vk_surface_format;

        VkInstance vk_instance;
        VkDevice vk_device;

        std::vector<VkImage> swapchain_images;
};