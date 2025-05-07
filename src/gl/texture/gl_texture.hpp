#pragma once

#include <glad/glad.h>

#include <cstdint>
#include <string>

#include "io/logging/logger.hpp"

namespace GMTKEngine {
    struct GLTextureData {

    };

    class GLTexture {
        public:
            GLTexture();
            GLTexture(std::string imagePath);

            GLTexture(const GLTexture& other) = delete; 

            GLTexture(GLTexture&& other); 
            void operator=(GLTexture&& other);

            ~GLTexture();

            GLuint getTexture();
            bool isValid();
        private:
            void createTexture(std::string path);

            GLuint tex;
    };
}