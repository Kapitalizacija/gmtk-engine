#pragma once

#include "al_buffer.hpp"
#include <AL/al.h>
#include <glm/glm.hpp>

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

            void setPosition(const glm::vec3 &pos);
            glm::vec3 getPosition() const { return mPosition; }

            ALuint getSourceID() const { return mSource; }
    
        private:
            ALuint mSource;
            glm::vec3 mPosition;
    };
}