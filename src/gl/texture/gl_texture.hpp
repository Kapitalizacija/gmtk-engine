#pragma once

#include <glad/glad.h>

#include <cstdlib>


namespace GMTKEngine {
    class GLTexture {
        public:
            GLTexture();

            GLuint get_texture();
            GLuint load_texture();

        private:
            int width, height, channels;

            GLuint tex;
    };
}