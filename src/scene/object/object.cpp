#include "object.hpp"

namespace GMTKEngine {
    Object::Object() {
        rendered = false;
    }

    Object::~Object() {
        for (auto &comp : mComponents) {
            delete comp.second;
        }
    }

    void Object::start() {
        for (auto &comp : mComponents) {
            comp.second->start();
        }
    }

    void Object::earlyUpdate() {
        for (auto &comp : mComponents) {
            comp.second->early_update();
        }
    }
    
    void Object::update() {
        for (auto &comp : mComponents) {
            comp.second->update();
        }
    }
    
    void Object::lateUpdate() {
        for (auto &comp : mComponents) {
            comp.second->late_update();
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