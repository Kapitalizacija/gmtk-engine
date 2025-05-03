#include "device.hpp"

Device::Device(VkInstance vk_instance): vk_instance(vk_instance) {
    pick_physical_device();
    create_logical_device();
    retrieve_queues();
}

void Device::pick_physical_device() {
    uint32_t physical_device_count;
    vkEnumeratePhysicalDevices(vk_instance, &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices;
    physical_devices.resize(physical_device_count);
    vkEnumeratePhysicalDevices(vk_instance, &physical_device_count, physical_devices.data());

    vk_physical_device = VK_NULL_HANDLE;

    for (VkPhysicalDevice& device : physical_devices) {
        VkPhysicalDeviceProperties properties;
       // VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceProperties(device, &properties);
       // vkGetPhysicalDeviceFeatures(device, &features);

        if ( properties.apiVersion < VK_MAKE_API_VERSION(0, 1, 3, 0) ) {
            continue;
        }

        if ( properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) {
            vk_physical_device = device;
            break;
        } 

        if ( properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ) {
            vk_physical_device = device;
        }
    }

    if ( vk_physical_device == VK_NULL_HANDLE ) {
        throw std::runtime_error("Failed to find a proper GPU (broke ass bi' lmao )");
    } 
}

std::vector<VkDeviceQueueCreateInfo> Device::get_queue_create_infos() {
    static float queue_priorities[64] = {1.0};

    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> properties;
    properties.resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count, properties.data());

    graphics_family = {};
    transfer_family = {};

    std::vector<VkDeviceQueueCreateInfo> create_infos;

    for (int i = 0; i < queue_family_count; i++) {
        if ( properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT == VK_QUEUE_GRAPHICS_BIT ) {

            graphics_family.found = true;
            graphics_family.current_queue = 0;
            graphics_family.queue_family_index = i;
            graphics_family.queues.resize(properties[i].queueCount);

        } else if ( properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT == VK_QUEUE_TRANSFER_BIT ) {

            transfer_family.found = true;
            transfer_family.current_queue = 0;
            transfer_family.queue_family_index = i;
            graphics_family.queues.resize(properties[i].queueCount);

        } else {
            continue;
        }


        VkDeviceQueueCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = i;
        info.queueCount = properties[i].queueCount;
        info.pQueuePriorities = queue_priorities;

        create_infos.push_back(info);
    }

    if ( !graphics_family.found ) {
        throw new std::runtime_error("Did not find suitable graphics family (are you even using a gpu?)");
    }

    return std::move(create_infos);
}

void Device::create_logical_device() {
    std::vector<VkDeviceQueueCreateInfo> queue_infos = get_queue_create_infos();

    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    device_info.pQueueCreateInfos = queue_infos.data();
    device_info.queueCreateInfoCount = queue_infos.size();

    if ( vkCreateDevice(vk_physical_device, &device_info, nullptr, &vk_device) ) {
        throw new std::runtime_error("Failed to create a Vulkan device");
    }
    
}

void Device::retrieve_queues() {
    for (int i = 0; i < graphics_family.queues.size(); i++) {
        vkGetDeviceQueue(vk_device, graphics_family.queue_family_index, i, &graphics_family.queues[i]);
    }

    if ( !transfer_family.found ) {
        return;
    }

    for (int i = 0; i < transfer_family.queues.size(); i++) {
        vkGetDeviceQueue(vk_device, transfer_family.queue_family_index, i, &transfer_family.queues[i]);
    }
}


VkDevice Device::get_logical_device() {
    return vk_device;
}

VkPhysicalDevice Device::get_physical_device() {
    return vk_physical_device;
}

VkQueue Device::get_graphics_queue() {
    return graphics_family.queues[graphics_family.current_queue++ % graphics_family.queues.size()];
}

VkQueue Device::get_transfer_queue() {
    if ( !transfer_family.found ) {
        return get_graphics_queue();
    }

    return transfer_family.queues[transfer_family.current_queue++ % transfer_family.queues.size()];
}

bool Device::transfer_queue_family_available() {
    return transfer_family.found;
}

uint32_t Device::get_graphics_family_index() {
    return graphics_family.queue_family_index;
}

uint32_t Device::get_transfer_family_index() {
    if ( !transfer_family.found ) {
        return graphics_family.queue_family_index;
    }

    return transfer_family.queue_family_index;
}

Device::~Device() {
    vkDestroyDevice(vk_device, nullptr);
}