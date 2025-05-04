#pragma once

#include <glad/glad.h>

#include "object.hpp"
#include "components/texture.hpp"

namespace GMTKEngine {
    class Object2D : Object, Texture {
        public:

        protected:
            GLuint texture;            
    };
}