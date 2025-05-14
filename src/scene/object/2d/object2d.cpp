#include "object2d.hpp"

namespace GMTKEngine {
    
    Object2D::Object2D() {

        createComponent<Transform2D>();
        createComponent<Texture>();
    }


    void Object2D::setShader(GLShader& shader) {
        if (rendered) {
            ERROR("Tried to change an object's shader while it's rendering is enabled");
            return;
        }

        program = shader.get_program();
    }

    std::vector<float> Object2D::getDrawData() {
        std::shared_ptr<Transform2D> transform = getComponentLock<Transform2D>().value();
        return {
            transform->getPosition().x, transform->getPosition().y,
            transform->getDepth(),
            transform->getRotation(),
            transform->getScale().x, transform->getScale().y
        };
    }
}