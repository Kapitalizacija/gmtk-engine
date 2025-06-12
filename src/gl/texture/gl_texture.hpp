#pragma once

#include <glad/glad.h>

#include <cstdint>
#include <string>

#include "io/logging/logger.hpp"

namespace Sierra {
    struct GLTextureData {

    };

    class GLTexture {
        friend class Object;

        public:
            GLTexture();
            GLTexture(std::string imagePath);

            GLTexture(uint8_t* data, int width, int height, int channels, int alignment = 4);

            void partialUpdate(uint8_t* data, int xOffset, int yOffset, int width, int height); 

            GLTexture(const GLTexture&) = delete; 

            GLTexture(GLTexture&& other) noexcept; 
            void operator=(GLTexture&& other) noexcept;

            ~GLTexture();

            GLuint getTexture();
            bool isValid();
        private:
            void createTexture(uint8_t* data, int width, int height, int channels);

            GLuint mTex;

            int mWidth;
            int mHeight;
            
            GLenum mFormat;
    };
}