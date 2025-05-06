#pragma once

#include "io/logging/logger.hpp"
#include <AL/al.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>

namespace GMTKEngine {
    class ALBuffer {
        public:
            ALBuffer();
            ~ALBuffer();

            bool loadFromFile(const std::string &filename);
            bool loadWAV(const std::string &filename);
            bool loadMP3(const std::string &filename);
            ALuint getID() const { return mBuffer; }
        
        private:
            ALuint mBuffer;
    };
}