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
            object->early_update();
        }
    
        for (auto& object: objects ) {
            object->update();
        }
    
        for (auto& object: objects ) {
            object->late_update();
        }
    
        for (auto& object: objects ) {
            object->draw();
        }
    }

    void Scene::add_object2d(Object2D* object) {
        auto& object_group = draw_batches_2d[object->program];
        
        bool found_group = false;
        for (auto& shader_group : object_group) {
            if (shader_group.find(object->mTextureID) == shader_group.end() && shader_group.size() >= 32) {
                continue;
            } 
            shader_group[object->mTextureID].insert(object);
            found_group = true;
            break;
        }

        if (!found_group) {
            object_group.emplace_back();
            object_group[object_group.size() - 1][object->mTextureID].insert(object);
        }
    }
    
    void Scene::remove_object2d(Object2D* object) {
        bool found = false;

        auto& object_group = draw_batches_2d[object->program];
        for (auto& shader_group : object_group) {

            if (shader_group.find(object->mTextureID) != shader_group.end()) {
                shader_group[object->mTextureID].erase(object);
                found = true;
                break;
            }
        }

        if (!found) {
            ERROR("Tried to remove nonexistant object");
        }

    }

    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1;
    }
}