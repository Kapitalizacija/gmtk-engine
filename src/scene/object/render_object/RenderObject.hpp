#pragma once

#include "scene/object/camera/camera.hpp"

namespace GMTKEngine {
    class RenderObject {
        friend class Scene;

        protected:
            virtual void draw(Camera& camera) = 0;
    };
}