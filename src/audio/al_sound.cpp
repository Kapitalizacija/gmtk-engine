#include "al_sound.hpp"

namespace GMTKEngine {
    ALSound::ALSound(ALBuffer &buf) {
        alGenSources(1, &mSource);
        alSourcei(mSource, AL_BUFFER, buf.getID());
    }

    ALSound::~ALSound() {
        alDeleteSources(1, &mSource);
    }

    void ALSound::play() {
        alSourcePlay(mSource);
    }

    void ALSound::pause() {
        alSourcePause(mSource);
    }

    void ALSound::stop() {
        alSourceStop(mSource);
    }

    void ALSound::setLooping(bool loop) {
        alSourcei(mSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

    void ALSound::setGain(float gain) {
        alSourcef(mSource, AL_GAIN, gain);
    }
}