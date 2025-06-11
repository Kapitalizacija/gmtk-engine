#pragma once

#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>

#include "io/logging/logger.hpp"
#include "gl/texture/gl_texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

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
            void initFreeType();
            void loadFontFace(std::string path);
            void load_glyphs();

            std::vector<uint8_t> readFont(std::string path);
            std::vector<uint8_t> createBitmap(std::vector<uint8_t>& buff, std::string fontPath);

            FT_Library library;
            FT_Face face;

            GLTexture fontBitmap;
            uint32_t bitmapWidth;
            uint32_t bitmapHeight;

            std::vector<std::pair<int, int>> charPositions;

            static const uint32_t CHAR_BOX_SIZE = 128;
            static const char START_CHAR = '!';
            static const char END_CHAR = 'z';
    };
}