#include "scene.hpp"

namespace GMTKEngine {
    Scene::Scene() {

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

        for (auto& object : objects ) {
            object->frameCleanup();
        }
    
    }

<<<<<<< Updated upstream
    void Scene::freeUnusedMemory() {
        renderer2d.freeUnusedMemory();
    }
    
    Camera* Scene::getCamera() {
        return &camera;
=======
    void Scene::add_object2d(Object2D* object) {
        if(object->mTextureID == 0) {
            ERROR("Tried to add object with invalid texture");
            return;
        }

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
        if(object->mTextureID == 0) {
            ERROR("Tried to remove object with invalid texture");
            return;
        }

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

>>>>>>> Stashed changes
    }

/*    uint64_t Scene::combine_hashes(uint64_t hash1, uint64_t hash2) {
        hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2); // stolen from boost
        return hash1c
    } might need in the future 
     dount remove*/
}