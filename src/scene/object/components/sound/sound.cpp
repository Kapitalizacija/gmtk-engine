#include "sound.hpp"

namespace GMTKEngine {
    //TODO: Needs testing
    Sound::Sound() {
        mSoundBuffers = {}; //Init the buffer map
    }

    Sound::~Sound() {
        //Clear the buffers and playing
        for (auto &snd : mPlayingSounds) {
            snd.second.reset();
        }

        for (auto &buf : mSoundBuffers) {
            buf.second.reset();
        }
    }

    void Sound::update() {
        for (auto it = mPlayingSounds.begin(); it != mPlayingSounds.end(); ) {
            if (it->second->getState() == AL_STOPPED) {
                it->second.reset();
                it = mPlayingSounds.erase(it); // Erase and advance
            } else {
                ++it;
            }
        }        
    }

    bool Sound::loadSound(const std::string &soundID, const std::string &fileName) {
        std::shared_ptr<ALBuffer> buffer(new ALBuffer());
        if (!buffer->loadFromFile(fileName)) {
            ERROR("Failed to load audio file: " << fileName);
            return false;
        }

        auto result = mSoundBuffers.insert({ soundID, buffer });
        if (result.second) {
            LOG("Loaded audio file: " << fileName << " as " << soundID);
            return true;
        } else {
            WARN("The requested soundID ('" << soundID << "') already exists on this Object. The sound will not be loaded.");
            buffer.reset();
            return false;
        }
    }

    void Sound::freeSound(const std::string &soundID) {
        auto it = mSoundBuffers.find(soundID);
        if (it != mSoundBuffers.end()) {
            mSoundBuffers.erase(it);
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
                std::shared_ptr<ALBuffer> ref = it->second;
                std::shared_ptr<ALSound> sound(new ALSound(*ref));
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
            std::shared_ptr<ALSound> ref = itP->second;
            ref->play();
        }
    }

    void Sound::pauseSound(const std::string &soundID) {
        auto itP = mPlayingSounds.find(soundID);
        if (itP != mPlayingSounds.end()) {
            std::shared_ptr<ALSound> ref = itP->second;
            ref->pause();
        } else {
            ERROR("soundID '" << soundID << "' was not found. Did not pause.");
        }
    }

    void Sound::stopSound(const std::string &soundID) {
        auto itP = mPlayingSounds.find(soundID);
        if (itP != mPlayingSounds.end()) {
            std::shared_ptr<ALSound> ref = itP->second;
            ref->stop();
        } else {
            ERROR("soundID '" << soundID << "' was not found. Did not stop.");
        }
    }
}