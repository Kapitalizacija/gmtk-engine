#include "transform3d.hpp"

namespace Sierra {
    namespace Component {
        Transform3D::Transform3D(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale, std::vector<Vertex> vertices) {
            mComponentName = "Transform";
            mPosition = position;
            mRotation = rotation;
            mScale = scale;
            mVertices = vertices;
            changed = false;
        }

        glm::vec3 Transform3D::getPosition() {
            return mPosition;
        }

        void Transform3D::setPosition(glm::vec3 newPos) {
            mPosition = newPos;
            changed = true;
        }

        glm::vec3 Transform3D::getScale() {
            return mScale;
        }

        void Transform3D::setScale(glm::vec3 newScale) {
            mScale = newScale;
            changed = true;
        }

        glm::vec4 Transform3D::getRotation() {
            return mRotation;
        }

        void Transform3D::setRotation(glm::vec4 newRotation) {
            mRotation = newRotation;
            changed = true;
        }
    
        bool Transform3D::hasChanged() {
            return changed;
        }

        void Transform3D::frameCleanup() {
            changed = false;
        }
    }
}