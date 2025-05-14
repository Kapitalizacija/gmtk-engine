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

            template<typename T>
            GLTexture(T* data, int width, int height, int channels) {
                createTexture((uint8_t*) data, width, height, channels);
            }

            GLTexture(const GLTexture&) = delete; 

            GLTexture(GLTexture&& other); 
            void operator=(GLTexture&& other);

            ~GLTexture();

            GLuint getTexture();
            bool isValid();
        private:
            void createTexture(uint8_t* data, int width, int height, int channels);

            GLuint tex;
    };
}