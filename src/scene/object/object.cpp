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

    void Object::earlyUpdate() {
        for (auto &comp : mComponents) {
            comp.second->earlyUpdate();
        }
    }
    
    void Object::update() {
        for (auto &comp : mComponents) {
            comp.second->update();
        }
    }
    
    void Object::lateUpdate() {
        for (auto &comp : mComponents) {
            comp.second->lateUpdate();
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