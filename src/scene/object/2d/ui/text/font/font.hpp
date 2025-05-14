#pragma once

#include <fstream>
#include <vector>

#include "gl/texture/gl_texture.hpp"

namespace GMTKEngine {
    class Font {
        public:
            Font(std::string fontPath);
        private:
            std::vector<unsigned char*> readFont(std::string path);

            GLTexture fontTex;
    };
}