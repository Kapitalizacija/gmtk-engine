#include "transform.hpp"

namespace GMTKEngine {
    Transform::Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale, std::vector<Vertex> vertices) {
        mComponentName = "Transform";
        mPosition = position;
        mRotation = rotation;
        mScale = scale;
        mVertices = vertices;
    }

    Transform::~Transform() {
        WARN("Nigga this shit ain't implemented");
    }
}