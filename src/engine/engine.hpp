#pragma once

#include "util/utilities.hpp"
#include "scene/scene.hpp"
#include "memory/mempool.hpp"
#include "window/window.hpp"
#include "audio/al_device.hpp"
#include "input/input.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Sierra {
    class Engine {
        public:
            Engine(std::string &windowTitle, glm::ivec2 dimensions, uint32_t mempoolSize);
            DISABLE_COPY_AND_MOVE(Engine);
            ~Engine();

            void start();
            void update();
            bool loadScene(Scene *scene);
            void unloadScene();

            Window *getWindow() { return mWindow; }
            Scene *getScene() { return mActiveScene; }
            Memory::Mempool *getMempool() { return mMempool; }
            ALDevice *getALDevice() { return mALDevice; }
        
        private: 
            Window *mWindow;
            Scene* mActiveScene;
            Memory::Mempool *mMempool;
            ALDevice *mALDevice;
    };
}