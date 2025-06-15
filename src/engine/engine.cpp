#include "engine.hpp"

namespace Sierra {
    Engine::Engine(std::string &windowTitle, glm::ivec2 dimensions, uint32_t mempoolSize) {
        mMempool = new Memory::Mempool(mempoolSize); //This is pretty stupid, but I can't be bothered to implement a better solution (a move constructor)
        LOG("Mempool init");
        
        mALDevice = new ALDevice(); //Should be moved to stack as well
        if (!mALDevice->isValid()) {
            ERROR("OpenAL Device Error");
        }

        Sierra::Resolution res;
        res.w = dimensions.x;
        res.h = dimensions.y;
        mWindow = new Window(windowTitle, res);
        LOG("Window init");
    }

    Engine::~Engine() {
        //Active scene is handled by stack, so we're not touching that shit
        if (mALDevice)
            delete mALDevice;
        if (mMempool)
            delete mMempool;
        if (mWindow)
            delete mWindow;
    }

    void Engine::start() {
        if (mActiveScene != nullptr) {
            mActiveScene->update();
        }
    }
        
    void Engine::update() {
        if (mWindow != nullptr) {
            mWindow->update();
        }
        
        if (mActiveScene != nullptr) {
            mActiveScene->update();
        }
    }

    bool Engine::loadScene(Scene *scene) {
        if (scene != nullptr) {
            mActiveScene = scene; //This should kill the objects in the previous scene (?) since they're smart pointers
            return true;
        }

        return false;
    }

    void Engine::unloadScene() {
        //mActiveScene = nullptr; //Setting to nullptr should nuke the scene (if the last owner)
    }        
}