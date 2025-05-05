#pragma once

#include "io/logging/logger.hpp"
#include <AL/al.h>
#include <string>
#include <iostream>
#include <vector>
#include <miniaudio.h>

namespace GMTKEngine {
    class ALBuffer {
        public:
            ALBuffer();
            ~ALBuffer();

            bool loadFromFile(const std::string &fileName);
            ALuint getID() const { return mBuffer; }
        
        private:
            ALuint mBuffer;
    };
}