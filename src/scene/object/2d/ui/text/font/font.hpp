#pragma once

#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>

#include "io/logging/logger.hpp"
#include "gl/texture/gl_texture.hpp"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace Sierra {
    
    class Font {
        friend class Text;

        public:
        
            Font(const Font&) = delete;
            Font(Font&&) = delete;
        
        protected:
            Font(std::string fontPath);

            std::vector<float> getCharOffsets(std::string text);
            GLTexture& getBitmap();            

        private:
            void initFreeType();
            void loadFontFace(std::string path);
            void loadGlyphs();

            std::vector<uint8_t> readFont(std::string path);
            std::vector<uint8_t> createBitmap(std::vector<uint8_t>& buff, std::string fontPath);

            FT_Library library;
            FT_Face face;

            GLTexture fontBitmap;
            uint32_t bitmapWidth;
            uint32_t bitmapHeight;

            std::vector<FT_GlyphSlotRec_> glyphs;

            static const uint32_t CHAR_BOX_SIZE = 128;
            static const char START_CHAR = 0;
            static const char END_CHAR = 126;
    };
}