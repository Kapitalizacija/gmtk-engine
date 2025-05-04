#pragma once

#include <glad/glad.h>


namespace GMTKEngine {
    class GLTexture {
        public:
            GLuint get_texture();
            GLuint load_texture();

        private:
            int width, height, channels;
    };
}