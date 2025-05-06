#pragma once

#include <glad/glad.h>

#include "../object.hpp"
#include "../components/texture/texture.hpp"
#include "../components/transform/transform.hpp"

namespace GMTKEngine {
    class Object2D : public Object, public Texture, public Transform {
        friend class Renderer2D;

        public:

        protected:
            std::vector<float> getDrawData() override;
    };
}