#pragma once

#include <glad/glad.h>

#include "../object.hpp"
#include "../components/texture/texture.hpp"

//TODO everything
namespace Sierra {
    class Object3D : Object {
        public:

        protected:
            GLuint get_texture();

            GLuint texture;            
    };
}