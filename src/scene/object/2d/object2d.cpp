#include "object2d.hpp"

namespace Sierra {
    

    Object2D::Object2D() {

        createComponent<Component::Transform2D>();
        createComponent<Component::Texture>();
    }


    void Object2D::setShader(GLShader& shader) {
        if (rendered) {
            ERROR("Tried to change an object's shader while it's rendering is enabled");
            return;
        }

        program = shader.getProgram();
    }

    std::vector<float> Object2D::getDrawData() {
        ResourceRef<Component::Transform2D> transform = getComponent<Component::Transform2D>();
        return {
            transform->getPosition().x, transform->getPosition().y, transform->getPosition().z,
            transform->getRotation(),
            transform->getScale().x, transform->getScale().y
        };
    }
}