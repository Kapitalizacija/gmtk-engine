#include "font.hpp"


namespace Sierra {

    Font::Font(std::string fontPath) {
        initFreeType();
        loadFontFace(fontPath);

        //fontBitmap = GLTexture(bitmapBuff.data(), bitmapWidth, bitmapHeight, 1);
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

        err = FT_Set_Pixel_Sizes(face, 128, 128);

        if (err)
            ERROR("Failed to set char size");
    }

    void Font::load_glyphs() {

        FT_Error err;
        for (int i = START_CHAR; i <= END_CHAR; i++) {
            FT_UInt index =  FT_Get_Char_Index(face, i);

            err = FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
            if (err) 
                ERROR("Failed to load glyph " << (char)i);

            err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO); // or normal idfk

            if (err)
                ERROR("Failed to render glyph" << (char)i);
        }
    }
    
    std::vector<uint8_t> Font::createBitmap(std::vector<uint8_t>& buff, std::string fontPath) {
       
    }
    
    std::vector<float> Font::getCharOffsets(std::string text) {
        std::vector<float> offsets;
        offsets.reserve(text.length() * 2);

        for(char c : text) {
            if (c < START_CHAR || c > END_CHAR) {
                if ((int)c != 32) {
                    ERROR("Character in text out of range: " << (int)c);
                }

                offsets.push_back(0);
                offsets.push_back(0);

                continue;
            }

            offsets.push_back(charPositions[c - START_CHAR].first / (float)bitmapWidth);
            offsets.push_back(charPositions[c - START_CHAR].second / (float)bitmapWidth);
        }

        return offsets;
    }

    GLTexture& Font::getBitmap() {
        return fontBitmap;
    }            
}