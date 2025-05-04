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
}