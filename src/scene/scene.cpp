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
        for (auto& object: objects ) {
            object->earlyUpdate();
        }
    
        for (auto& object: objects ) {
            object->update();
        }
    
        for (auto& object: objects ) {
            object->lateUpdate();
        }
    
        for (auto& object: objects ) {
            object->draw();
        }
    }



/*    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1;
    } might need in the future 
     dount remove*/
}