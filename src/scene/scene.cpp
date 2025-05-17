#include "scene.hpp"

namespace GMTKEngine {
    Scene::Scene(): sinceLastFixedUpdate(0) {
        renderer = std::make_shared<Renderer>();
    }

    void Scene::start() {
        for (auto &object : objects) {
            object->start();
        }
    }
    
    void Scene::update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->camera->update();

        for (auto& object: objects ) {
            object->earlyUpdate();
        }
    
        for (auto& object: objects ) {
            object->update();
        }
    
        for (auto& object: objects ) {
            object->lateUpdate();
        }

        fixedUpdate();

        renderer->renderer();

        for (auto& object : objects ) {
            object->frameCleanup();
        }
    
    }

    void Scene::fixedUpdate() {
        if (CLOCK_MS - sinceLastFixedUpdate < FIXED_UPDATE_INTERVAL) {
            return;
        }

        sinceLastFixedUpdate = CLOCK_MS;

        for (auto& object: objects ) {
            object->fixedUpdate();
        }
    }

    void Scene::freeUnusedMemory() {

    }
    
    std::weak_ptr<Camera> Scene::getCamera() {
        return renderer->camera;
    }

    std::weak_ptr<Renderer> getRenderer();

/*    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1c
    } might need in the future 
     dount remove*/
}