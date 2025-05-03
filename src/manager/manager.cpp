#include "manager.hpp"

namespace GMTKEngine {
    Manager::Manager() {

    }

    Manager::~Manager() {
        for (auto &object : objects) {
            delete object;
        }
    }

    void Manager::start() {
        for (auto &object : objects) {
            object->start();
        }
    }
    
    void Manager::update() {
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