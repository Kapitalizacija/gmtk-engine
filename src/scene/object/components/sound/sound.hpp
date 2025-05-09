#pragma once

#include <unordered_map>
#include "../component.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"

namespace GMTKEngine {
    class Sound : public Component {
        public:
            Sound();
            ~Sound();

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update();
            virtual void late_update() { return; }

            void setPosition(const glm::vec3 &pos) { mPosition = pos; }
            glm::vec3 getPosition() const { return mPosition; }
            void setIsLooping(const bool loop) { mIsLooping = loop; }
            bool getIsLooping() const { return mIsLooping; }
            void setGain(const float gain) { mGain = gain; }
            bool getGain() const { return mGain; }

            //Loads the desired sound from path as soundId. Returns success status.
            bool loadSound(const std::string &soundID, const std::string &fileName);
            //Unloads the desired sound.
            void freeSound(const std::string &soundID);
            //Plays the desired sound that has been pre-loaded.
            void playSound(const std::string &soundID);
            //Pauses the desired sound that has been pre-loaded.
            void pauseSound(const std::string &soundID);
            //Stops the desired sound that has been pre-loaded.
            void stopSound(const std::string &soundID);

        private:
            glm::vec3 mPosition;
            bool mIsLooping;
            float mGain;

            std::unordered_map<std::string, ALBuffer*> mSoundBuffers;
            std::unordered_map<std::string, ALSound*> mPlayingSounds;
    };
}