#include "sound.hpp"

namespace GMTKEngine {
    //TODO: Needs testing
    Sound::Sound() {
        mSoundBuffers = {}; //Init the buffer map
    }

    Sound::~Sound() {
        //Clear the buffers and playing
        for (auto &snd : mPlayingSounds) {
            delete snd.second;
        }

        for (auto &buf : mSoundBuffers) {
            delete buf.second;
        }
    }

    void Sound::update() {
        for (auto &snd : mPlayingSounds) {
            if (snd.second->getState() == AL_STOPPED) {
                //Delete handles to stopped sounds
                delete snd.second;
            }
        }
    }

    bool Sound::loadSound(const std::string &soundID, const std::string &fileName) {
        ALBuffer *buffer = new ALBuffer;
        if (!buffer->loadFromFile(fileName)) {
            ERROR("Failed to load audio file: " << fileName);
            delete buffer;
            return false;
        }

        auto result = mSoundBuffers.insert({ soundID, buffer });
        if (result.second) {
            LOG("Loaded audio file: " << fileName << " as " << soundID);
            return true;
        } else {
            WARN("The requested soundID ('" << soundID << "') already exists on this Object. The sound will not be loaded.");
            delete buffer;
            return false;
        }
    }

    void Sound::freeSound(const std::string &soundID) {
        auto it = mSoundBuffers.find(soundID);
        if (it != mSoundBuffers.end()) {
            delete it->second;
            LOG("Successfully freed the sound '" << soundID << "'");
        } else {
            WARN("soundID '" << soundID << "' was not found. Did not free.");
        }
    }

    //TODO: Apply changes to gain, looping etc. during playtime.
    void Sound::playSound(const std::string &soundID) {
        auto itP = mPlayingSounds.find(soundID);
        if (itP == mPlayingSounds.end()) {
            //Not existing yet
            auto it = mSoundBuffers.find(soundID);
            if (it != mSoundBuffers.end()) {
                ALBuffer *ref = it->second;
                ALSound *sound = new ALSound(*ref);
                sound->setPosition(mPosition);
                sound->setGain(mGain);
                sound->setLooping(mIsLooping);

                sound->play();
                mPlayingSounds.insert({ soundID, sound }); //Store the reference
            } else {
                ERROR("soundID '" << soundID << "' was not found. Did not play.");
            }
        } else {
            //Exists already
            ALSound *ref = itP->second;
            ref->play();
        }
    }

    void Sound::pauseSound(const std::string &soundID) {
        auto itP = mPlayingSounds.find(soundID);
        if (itP != mPlayingSounds.end()) {
            ALSound *ref = itP->second;
            ref->pause();
        } else {
            ERROR("soundID '" << soundID << "' was not found. Did not pause.");
        }
    }

    void Sound::stopSound(const std::string &soundID) {
        auto itP = mPlayingSounds.find(soundID);
        if (itP != mPlayingSounds.end()) {
            ALSound *ref = itP->second;
            ref->stop();
        } else {
            ERROR("soundID '" << soundID << "' was not found. Did not stop.");
        }
    }
}