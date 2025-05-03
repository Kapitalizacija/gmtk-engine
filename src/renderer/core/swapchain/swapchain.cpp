#include "swapchain.hpp"


Swapchain::Swapchain(SwapchainCreateInfo& create_info):
 vk_swapchain(VK_NULL_HANDLE), vk_device(create_info.device.get_logical_device()), vk_instance(create_info.vk_instance) {
    create_surface(create_info.glfw_window, create_info.vk_instance);
    retrieve_surface_capabilities(create_info.device.get_physical_device(), create_info.preferred_image_count);
    pick_surface_format(create_info.device.get_physical_device());
    create_swapchain(create_info.device);
    retrieve_swapchain_images();
}

Swapchain::Swapchain(SwapchainCreateInfo&& create_info): // because g++ is a little whiny bitch
 vk_swapchain(VK_NULL_HANDLE), vk_device(create_info.device.get_logical_device()), vk_instance(create_info.vk_instance) {
    create_surface(create_info.glfw_window, create_info.vk_instance);
    retrieve_surface_capabilities(create_info.device.get_physical_device(), create_info.preferred_image_count);
    pick_surface_format(create_info.device.get_physical_device());
    create_swapchain(create_info.device);
    retrieve_swapchain_images();
}

void Swapchain::create_surface(GLFWwindow* glfw_window, VkInstance vk_instance) {
    if ( glfwCreateWindowSurface(vk_instance, glfw_window, nullptr, &vk_surface) != VK_SUCCESS ) {
        throw new std::runtime_error("Failed to create a window surface");
    }
}

void Swapchain::retrieve_surface_capabilities(VkPhysicalDevice vk_physical_device, uint32_t preferred_image_count) {
    VkSurfaceCapabilitiesKHR vk_capabilties;
    if ( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physical_device, vk_surface, &vk_capabilties) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to retrieve the window surface capabilties");
    }

    capabilities.image_count = std::clamp(preferred_image_count, vk_capabilties.minImageCount, vk_capabilties.minImageCount);
    capabilities.extent = vk_capabilties.currentExtent;

    assert(vk_capabilties.currentExtent.width != 0xFFFFFFFF); //for now
}

void Swapchain::pick_surface_format(VkPhysicalDevice vk_physical_device) {
    uint32_t surface_format_count;
    if ( vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &surface_format_count, nullptr) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to retrieve surface format count");
    }

    assert(surface_format_count > 0);

    std::vector<VkSurfaceFormatKHR> surface_formats;
    surface_formats.resize(surface_format_count);
    if ( vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &surface_format_count, surface_formats.data()) != VK_SUCCESS ) {
        throw std::runtime_error("Failed to retrieve surface formats");
    }

    vk_surface_format = surface_formats[0];

    for ( VkSurfaceFormatKHR& format : surface_formats ) {
        if ( format.format == VK_FORMAT_R8G8B8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR ) {
            vk_surface_format = format;
            break;
        }
    }

}

void Swapchain::create_swapchain(Device& device) {
    uint32_t indices[] = {
        device.get_graphics_family_index(),
        device.get_transfer_family_index()
    };

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = vk_surface;
    create_info.minImageCount = capabilities.image_count;
    create_info.imageFormat = vk_surface_format.format;
    create_info.imageColorSpace = vk_surface_format.colorSpace;
    create_info.imageExtent = capabilities.extent;
    create_info.imageArrayLayers = 1;

    if ( !device.transfer_queue_family_available() ) {
        create_info.imageUsage = VK_SHARING_MODE_EXCLUSIVE;
    } else {
        create_info.imageUsage = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = indices;
    }

    create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = vk_swapchain;

    if ( vkCreateSwapchainKHR(device.get_logical_device(), &create_info, nullptr, &vk_swapchain) != VK_SUCCESS) {
        throw new std::runtime_error("Failed to create a new swapchain");
    }

}

void Swapchain::retrieve_swapchain_images() {
    uint32_t image_count;
    if ( vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &image_count, nullptr) != VK_SUCCESS) {
        throw new std::runtime_error("Faield to get swapchain image count");
    }

    swapchain_images.resize(image_count);
    if ( vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &image_count, swapchain_images.data()) != VK_SUCCESS) {
        throw new std::runtime_error("Faield to get swapchain images");
    }
}

uint32_t Swapchain::get_swapchain_image_count() {
    return swapchain_images.size();
}

Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);
    vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
}