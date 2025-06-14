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

    void Object::earlyUpdate(const StateInfo& stateInfo) {
        for (auto &comp : mComponents) {
            comp.second->earlyUpdate(stateInfo);
        }
    }
    
    void Object::update(const StateInfo& stateInfo) {
        for (auto &comp : mComponents) {
            comp.second->update(stateInfo);
        }
    }
    
    void Object::lateUpdate(const StateInfo& stateInfo) {
        for (auto &comp : mComponents) {
            comp.second->lateUpdate(stateInfo);
        }
    }

    void Object::fixedUpdate(const StateInfo& stateInfo) {
        for (auto &comp : mComponents) {
            comp.second->fixedUpdate(stateInfo);
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