#include "scene.hpp"

namespace GMTKEngine {
    Scene::Scene() {

    }

    Scene::~Scene() {
        for (auto &object : objects) {
            delete object;
        }
    }

    void Scene::start() {
        for (auto &object : objects) {
            object->start();
        }
    }
    
    void Scene::update() {
        camera.update();

        for (auto& object: objects ) {
            object->earlyUpdate();
        }
    
        for (auto& object: objects ) {
            object->update();
        }
    
        for (auto& object: objects ) {
            object->lateUpdate();
        }

        renderer2d.update();

        renderer2d.render(camera);

        for (auto& object: objects ) {
            object->frameCleanup();
        }
    
    }

    void Scene::freeUnusedMemory() {
        renderer2d.freeUnusedMemory();
    }
    
    Camera* Scene::getCamera() {
        return &camera;
    }

/*    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1c
    } might need in the future 
     dount remove*/
}