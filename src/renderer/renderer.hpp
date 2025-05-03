#pragma once

#include "core/core.hpp"
#include "../window/window.hpp"

class Renderer {
    public:
        Renderer(Window& window);

        Renderer(Renderer& other) = delete;
        Renderer(Renderer&& other) = delete;

        ~Renderer();

    private:
        Core core;
};