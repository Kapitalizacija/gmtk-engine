#pragma once

#include <glad/glad.h>

#include "object.hpp"
#include "components/texture.hpp"

//TODO everything
namespace GMTKEngine {
    class Object3D : Object, Texture {
        public:

        protected:
            GLuint get_texture();

            GLuint texture;            
    };
}