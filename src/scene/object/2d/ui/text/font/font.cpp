#include "font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype/stb_truetype.h>

namespace Sierra {

    Font::Font(std::string fontPath) {
        std::vector<uint8_t> rawBuff = readFont(fontPath);
        if (rawBuff.empty()) {
            return;
        }

        std::vector<uint8_t> bitmapBuff = createBitmap(rawBuff, fontPath);
        if(bitmapBuff.empty()) {
            return;
        }

        fontBitmap = GLTexture(bitmapBuff.data(), bitmapWidth, bitmapHeight, 1);
    }
    
    std::vector<uint8_t> Font::createBitmap(std::vector<uint8_t>& buff, std::string fontPath) {
        stbtt_fontinfo info;
        if (!stbtt_InitFont(&info, buff.data(), 0)) {
            ERROR("Failed to parse font " << fontPath);
            return {};
        }
        
        int x0, x1, y0, y1;
        bitmapHeight = CHAR_HEIGHT;
        bitmapWidth = 0;

        int maxHeight = 0;
        for (int i = START_CHAR; i <= END_CHAR; i++) {
            stbtt_GetCodepointBox(&info, i, &x0, &y0, &x1, &y1);
            bitmapWidth += x1 - x0;
            maxHeight = std::max(maxHeight, y1 - y0);
        }

        bitmapWidth = ceil((float)bitmapWidth * ((float)bitmapHeight / maxHeight));


        std::vector<uint8_t> bitmapBuff;
        bitmapBuff.resize(bitmapHeight * bitmapWidth);
        std::vector<stbtt_packedchar> charInfo;
        charInfo.resize(END_CHAR - START_CHAR);

        stbtt_pack_context context{};
        int res = stbtt_PackBegin(&context, bitmapBuff.data(), bitmapWidth, bitmapHeight, 0, 1, nullptr);

        if (!res) {
            ERROR("Failed to create font context for " << fontPath);
            return {};
        }

        res = stbtt_PackFontRange(&context, buff.data(), 0, bitmapHeight, START_CHAR, (END_CHAR - START_CHAR), charInfo.data());

        if (!res) {
            ERROR("Failed to pack font: " << fontPath);
            stbtt_PackEnd(&context);
            return {};
        }

        stbtt_PackEnd(&context);

        charPositions.reserve(charInfo.size());;
        for (stbtt_packedchar& info : charInfo) {
            charPositions.push_back( {info.x0, info.x1} );
        }

        return bitmapBuff;
    }

    std::vector<uint8_t> Font::readFont(std::string path) {
        std::fstream stream(path, std::ios::ate | std::ios_base::in);

        if (!stream.is_open()) {
            ERROR("Failed to open font: " << path);
            return {};
        }

        size_t size = stream.tellg();
        stream.seekg(0);

        std::vector<uint8_t> buff;
        buff.resize(size);

        stream.read((char*)buff.data(), size);
        stream.close();

        return buff;
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