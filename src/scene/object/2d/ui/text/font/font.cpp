#include "font.hpp"


namespace Sierra {

    Font::Font(std::string fontPath) {
        initFreeType();
        loadFontFace(fontPath);
        load_glyphs();

        bitmapWidth = 10 * CHAR_BOX_SIZE;
        bitmapHeight = 9 * CHAR_BOX_SIZE;
        fontBitmap = GLTexture(nullptr, bitmapWidth, bitmapHeight, 1);
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

        err = FT_Set_Pixel_Sizes(face, CHAR_BOX_SIZE, CHAR_BOX_SIZE);

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

            uint32_t x = (i % 10) * CHAR_BOX_SIZE;
            uint32_t y = floor(i / 10) * CHAR_BOX_SIZE;

           fontBitmap.partialUpdate(face->glyph->bitmap.buffer, x, y); 
        }
    }
    
    std::vector<float> Font::getCharOffsets(std::string text) {
        std::vector<float> offsets;
        offsets.reserve(text.length() * 2);

        for(char c : text) {
            if (c < START_CHAR || c > END_CHAR) {
                continue;
            }


            uint32_t x = ((c - START_CHAR) % 10) * CHAR_BOX_SIZE;
            uint32_t y = floor((c - START_CHAR) / 10) * CHAR_BOX_SIZE;

            if ((int)c == 32) {
                x = 0;
                y = 0;
            }

            offsets.push_back(x);
            offsets.push_back(y);
        }

        return offsets;
    }

    GLTexture& Font::getBitmap() {
        return fontBitmap;
    }            
}