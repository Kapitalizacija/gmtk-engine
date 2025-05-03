#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "../instance/instance.hpp"

#include <vulkan/vulkan.h>

struct QueueFamily {
    std::vector<VkQueue> queues;
    
    uint32_t queue_family_index;
    
    uint32_t current_queue;
    bool found;
};

class Device {
    public:
        Device(VkInstance vk_instance);

        Device(Device& other) = delete;
        Device(Device&& other) = delete;

        ~Device();

        VkDevice get_logical_device();
        VkPhysicalDevice get_physical_device();

        VkQueue get_graphics_queue();
        VkQueue get_transfer_queue();

        uint32_t get_graphics_family_index();
        uint32_t get_transfer_family_index();

        bool transfer_queue_family_available();

    private:
        void pick_physical_device();
        std::vector<VkDeviceQueueCreateInfo> get_queue_create_infos();
        void create_logical_device();
        void retrieve_queues();

        VkInstance vk_instance;

        VkPhysicalDevice vk_physical_device;
        VkDevice vk_device;

        QueueFamily transfer_family;
        QueueFamily graphics_family;
};