#include "object2D.hpp"

namespace GMTKEngine {
    
    Object2D::Object2D() {
        Transform* transform;
        Texture* texture;

        createComponent(&transform);
        createComponent(&texture);
    }


    void Object2D::setShader(GLShader& shader) {
        if (rendered) {
            ERROR("Tried to change an object's shader while it's rendering is enabled");
            return;
        }

        program = shader.get_program();
    }

    std::vector<float> Object2D::getDrawData() {
        auto x = std::string("ew");
        Transform* transform = (Transform*)mComponents[x];
        return {
            transform->getPosition().x, transform->getPosition().y, transform->getPosition().z,
            transform->getRotation().x, transform->getRotation().y, transform->getRotation().z, transform->getRotation().w,
            transform->getScale().x,    transform->getScale().y,  transform->getScale().z  
        };
    }

    bool Object2D::changedSinceLastCheck() {
        return changedSinceLastUpdate();
    }
}