#include "object.hpp"

namespace Sierra {
    Object::Object() {
        rendered = false;
    }

    void Object::start() {
        for (auto &comp : mComponents) {
            comp.second->start();
        }
    }

    void Object::earlyUpdate(float dt) {
        for (auto &comp : mComponents) {
            comp.second->earlyUpdate(dt);
        }
    }
    
    void Object::update(float dt) {
        for (auto &comp : mComponents) {
            comp.second->update(dt);
        }
    }
    
    void Object::lateUpdate(float dt) {
        for (auto &comp : mComponents) {
            comp.second->lateUpdate(dt);
        }
    }

    void Object::fixedUpdate() {
        for (auto &comp : mComponents) {
            comp.second->fixedUpdate();
        }
    }
    
    std::vector<float> Object::getDrawData() {
        ERROR("get_draw_data method not implemented, object instances won't be able to be drawn");
        return {};
    }
    
    void Object::frameCleanup() {
        for (auto& comp : mComponents) {
            comp.second->frameCleanup();
        }

        changed = false;
    }


    bool Object::isRendered() {
        return rendered;
    }

    bool Object::hasChanged() {
        return changed;
    }
    
}