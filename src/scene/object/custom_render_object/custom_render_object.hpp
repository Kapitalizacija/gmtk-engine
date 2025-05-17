#pragma once

#include "scene/object/camera/camera.hpp"

namespace GMTKEngine {
    class CustomRenderObject {
        friend class Scene;
        friend class Renderer;

        protected:
            virtual void draw(Camera& camera) = 0;
    };
}