#pragma once

#include "instance/instance.hpp"
#include "device/device.hpp"
#include "swapchain/swapchain.hpp"
#include "../../window/window.hpp"

class Core {
    public:
        Core(Window& window);

        Core(Core& other) = delete;
        Core(Core&& other) = delete;

        ~Core();

    private:
        Instance instance;
        Device device;
        Swapchain swapchain;
};