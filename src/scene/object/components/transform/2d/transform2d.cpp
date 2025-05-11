#include "transform2d.hpp"

namespace GMTKEngine {

    Transform2D::Transform2D(glm::vec2 position, float rotation, float depth, glm::vec2 scale) {
        this->mPosition = position;
        this->mScale = scale;
        this->rotation = rotation;
        this->depth = depth;
        this->changed = false;
    }

    glm::vec2 Transform2D::getPosition() {
        return mPosition;
    }

    void Transform2D::setPosition(glm::vec2 newPos) {
        changed = true;
        mPosition = newPos;
    }

    void Transform2D::translate(glm::vec2 vec) {
        changed = true;
        mPosition += vec;
    }

    glm::vec2 Transform2D::getScale() {
        return mScale;
    }

    void Transform2D::setScale(glm::vec2 newScale){
        changed = true;
        mScale = newScale;
    } 

    float Transform2D::getRotation() {
        return rotation;
    }

    void Transform2D::setRotation(float newRotation) {
        changed = true;
        rotation = newRotation;
    }
    
    void Transform2D::rotate(float amount) {
        rotation += amount;
    }


    float Transform2D::getDepth() {
        return depth;
    }

    void Transform2D::setDepth(float newDepth) {
        changed = true;
        depth = newDepth;
    }

    bool Transform2D::hasChanged() {
        return changed;
    }

    void Transform2D::frameCleanup() {
        changed = false;
    }
}