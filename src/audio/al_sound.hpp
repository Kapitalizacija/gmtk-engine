#pragma once

#include "al_buffer.hpp"
#include <AL/al.h>

namespace GMTKEngine {
    class ALSound {
        public:
            ALSound(ALBuffer &buf);
            ~ALSound();

            void play();
            void pause();
            void stop();
            void setLooping(bool loop);
            void setGain(float gain);
    
        private:
            ALuint mSource;
    };
}