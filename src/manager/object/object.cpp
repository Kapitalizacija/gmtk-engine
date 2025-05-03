#include "object.hpp"

namespace GMTKEngine {
    Object::Object() {
        LOG("Object Awoke");
    }

    Object::~Object() {
        for (auto &comp : mComponents) {
            delete comp;
        }
    }

    void Object::start() {
        for (auto &comp : mComponents) {
            comp->start();
        }
    }

    void Object::early_update() {
        for (auto &comp : mComponents) {
            comp->early_update();
        }
    }
    
    void Object::update() {
        for (auto &comp : mComponents) {
            comp->update();
        }
    }
    
    void Object::late_update() {
        for (auto &comp : mComponents) {
            comp->late_update();
        }
    }
    
    void Object::draw() {
        std::cerr << "ERROR: draw not implemented\n";
    }
}