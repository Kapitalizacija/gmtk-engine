#pragma once

#include "scene/object/camera/camera.hpp"

namespace GMTKEngine {
    class CustomRenderObject {
        friend class Scene;

        protected:
            virtual void draw(Camera& camera) = 0;
    };
}