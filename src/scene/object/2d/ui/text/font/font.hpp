#pragma once

#include <fstream>
#include <vector>
#include <tuple>


#include "io/logging/logger.hpp"
#include "gl/texture/gl_texture.hpp"

namespace Sierra {
    
    class Font {
        friend class Text;

        public:
            Font(std::string fontPath);

            Font(const Font&) = delete;
            Font(Font&&) = delete;

        protected:
            std::vector<float> getCharOffsets(std::string text);
            GLTexture& getBitmap();            

        private:
            std::vector<uint8_t> readFont(std::string path);
            std::vector<uint8_t> createBitmap(std::vector<uint8_t>& buff, std::string fontPath);

            GLTexture fontBitmap;
            uint32_t bitmapWidth;
            uint32_t bitmapHeight;

            std::vector<std::pair<int, int>> charPositions;

            static const uint32_t CHAR_HEIGHT = 256;
            static const char START_CHAR = '!';
            static const char END_CHAR = 'z';

    };
}