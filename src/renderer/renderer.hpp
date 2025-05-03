#pragma once

#include "../window/window.hpp"

namespace GMTKEngine {
    class Renderer {
        public:
            Renderer(Window& window);
    
            Renderer(Renderer& other) = delete;
            Renderer(Renderer&& other) = delete;
    
            ~Renderer();
    
        private:
    };
}