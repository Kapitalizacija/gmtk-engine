#include "font.hpp"


namespace Sierra {

    Font::Font(std::string fontPath) {
        bitmapWidth = 14 * CHAR_BOX_SIZE;
        bitmapHeight = 10 * CHAR_BOX_SIZE;

        fontBitmap = GLTexture(nullptr, bitmapWidth, bitmapHeight, 1, 1);

        initFreeType();
        loadFontFace(fontPath);
        loadGlyphs();

    }

    void Font::initFreeType() {
        FT_Error err = FT_Init_FreeType(&library);

        if (err) 
            ERROR("Failed to init the FreeType library");
       
    }

    void Font::loadFontFace(std::string path) {
        FT_Error err = FT_New_Face( library,
                                    path.c_str(),
                                    0,
                                    &face);

        if (err)
            ERROR("Failed to load a new font face");

        err = FT_Set_Pixel_Sizes(face, 0, CHAR_BOX_SIZE);

        if (err)
            ERROR("Failed to set char size");
    }

    void Font::loadGlyphs() {
        glyphs.resize(END_CHAR - START_CHAR + 1);

        FT_Error err;
        for (int i = START_CHAR; i <= END_CHAR; i++) {
            err = FT_Load_Char(face, i, FT_LOAD_RENDER);
            if (err) 
                ERROR("Failed to load glyph " << (char)i);

            if (face->glyph->bitmap.buffer == nullptr)
                continue;

            uint32_t x = ((i - START_CHAR) % 14) * CHAR_BOX_SIZE;
            uint32_t y = floor((i - START_CHAR) / 14) * CHAR_BOX_SIZE;

           fontBitmap.partialUpdate(face->glyph->bitmap.buffer, x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows); 

           glyphs[i - START_CHAR] = *face->glyph;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }
    
    std::vector<float> Font::getCharOffsets(std::string text) {
        std::vector<float> charDat;
        charDat.reserve(text.length() * 7);

        for(char c : text) {

            if (c == ' ') {
                charDat.push_back(0);
                charDat.push_back(0);
                charDat.push_back(0);
                charDat.push_back(0);
                charDat.push_back(0.5);
                charDat.push_back(0);
                charDat.push_back(0);

                continue;
            }

            const FT_GlyphSlotRec_& glyph = glyphs[c - START_CHAR];

            float x = ((c - START_CHAR) % 14) * CHAR_BOX_SIZE / (float)bitmapWidth;
            float y = floor((c - START_CHAR) / 14) * CHAR_BOX_SIZE / (float)bitmapHeight;

            charDat.push_back(x);
            charDat.push_back(y);

            charDat.push_back(x + CHAR_BOX_SIZE / (float)bitmapWidth);
            charDat.push_back(y + CHAR_BOX_SIZE / (float)bitmapHeight);

            charDat.push_back(glyph.advance.x / 64.0f / (float)CHAR_BOX_SIZE);
            
            charDat.push_back((float)glyph.bitmap_left / (float)CHAR_BOX_SIZE);
            charDat.push_back((float)glyph.bitmap_top / (float)CHAR_BOX_SIZE);
        }

        return charDat;
    }

    GLTexture& Font::getBitmap() {
        return fontBitmap;
    }            
}