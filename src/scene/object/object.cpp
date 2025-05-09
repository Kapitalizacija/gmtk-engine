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

    bool Object::isRendered() {
        return rendered;
    }

    bool Object::changedSinceLastUpdate() {
        for (auto& comp : mComponents) {
            if (comp.second->changedSinceLastUpdate()) {
                return true;
            }
        }
    }
    
    Component* Object::getComponent(std::string componentName) {
        DBG(mComponents.at(componentName));
        return mComponents.at(componentName);
    }
}