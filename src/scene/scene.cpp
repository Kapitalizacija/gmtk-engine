#include "scene.hpp"

namespace Sierra {
    Scene::Scene(ResourceRef<Window> window): mInputHandler(window), sinceLastFixedUpdate(0),
     mStateInfo(mInputHandler, 0) {
        GLUtils::init();

        renderer = std::make_shared<Renderer>();
        physicsManager2D = std::make_shared<PhysicsManager2D>();

        Debug::Renderer::init(renderer->mCamera);
    }

    void Scene::start() {
        for (auto &object : objects) {
            object->start();
        }
    }
    
    void Scene::update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dt.update();

        mStateInfo.dt = dt.get();

        renderer->mCamera->update(mStateInfo); 


        for (auto& object: objects ) {
            object->earlyUpdate(mStateInfo);
        }
    
        for (auto& object: objects ) {
            object->update(mStateInfo);
        }
    
        for (auto& object: objects ) {
            object->lateUpdate(mStateInfo);
        }

        fixedUpdate();

        physicsManager2D->update();
        
        renderer->renderer();
        
        for (auto& object : objects ) {
            object->frameCleanup();
        }

        mInputHandler.endFrame();
        
    }
    
    void Scene::fixedUpdate() {
        if (CLOCK_MS - sinceLastFixedUpdate < FIXED_UPDATE_INTERVAL) {
            return;
        }
        
        sinceLastFixedUpdate = CLOCK_MS;
        
        for (auto& object: objects ) {
            object->fixedUpdate(mStateInfo);
        }
    }

    void Scene::freeUnusedMemory() {

    }
    
    ResourceRef<Camera> Scene::getCamera() {
        return renderer->mCamera;
    }
    
    ResourceRef<Renderer> Scene::getRenderer() {
        return renderer;
    }

    float Scene::getDeltaTime() {
        return dt.get();
    }

    ResourceRef<PhysicsManager2D> Scene::getPhysicsManager2D() {
        return physicsManager2D;
    }

    std::weak_ptr<Renderer> getRenderer();

/*    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1c
    } might need in the future 
     dount remove*/
}