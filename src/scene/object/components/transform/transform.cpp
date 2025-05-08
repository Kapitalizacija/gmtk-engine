#include "transform.hpp"

namespace GMTKEngine {
    Transform::Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale, std::vector<Vertex> vertices) {
        mComponentName = "Transform";
        mPosition = position;
        mRotation = rotation;
        mScale = scale;
        mVertices = vertices;
    }

    glm::vec3 Transform::getPosition() {
        return mPosition;
    }

    void Transform::setPosition(glm::vec3 newPos) {
        mPosition = newPos;
    }

    glm::vec3 Transform::getScale() {
        return mScale;
    }

    void Transform::setScale(glm::vec3 newScale) {
        mScale = newScale;
    }

    glm::vec4 Transform::getRotation() {
        return mRotation;
    }

    void Transform::setRotation(glm::vec4 newRotation) {
        mRotation = newRotation;
    }

    Transform::~Transform() {
        WARN("Nigga this shit ain't implemented");
    }
}