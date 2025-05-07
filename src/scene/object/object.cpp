#include "object.hpp"

namespace GMTKEngine {
    Object::Object() {
        rendered = false;
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

    void Object::earlyUpdate() {
        for (auto &comp : mComponents) {
            comp->early_update();
        }
    }
    
    void Object::update() {
        for (auto &comp : mComponents) {
            comp->update();
        }
    }
    
    void Object::lateUpdate() {
        for (auto &comp : mComponents) {
            comp->late_update();
        }
    }
    
    std::vector<float> Object::getDrawData() {
        ERROR("get_draw_data method not implemented, object instances won't be able to be drawn");
        return {};
    }

    bool Object::isRendered() {
        return rendered;
    }
}