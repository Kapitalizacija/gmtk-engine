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
            Engine(std::string &windowTitle, Resolution dimensions, uint32_t mempoolSize);
            DISABLE_COPY_AND_MOVE(Engine);

            void start();
            void update();
            bool loadScene(Scene *scene);
            void unloadScene();

            ResourceRef<Window> getWindow() { return &mWindow; }
            ResourceRef<Scene> getScene() { return mActiveScene; }
            ResourceRef<Memory::Mempool> getMempool() { return &mMempool; }
            ResourceRef<ALDevice> getALDevice() { return &mALDevice; }
        
        private: 
            Window mWindow;
            ALDevice mALDevice;
            Memory::Mempool mMempool;
            ResourceRef<Scene> mActiveScene;
    };
}