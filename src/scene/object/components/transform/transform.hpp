#pragma once

#include "../component.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace GMTKEngine {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class Transform : public Component {
        public:
            Transform(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec4 rotation = glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(0.f, 0.f, 0.f), std::vector<Vertex> vertices = {});
            ~Transform();

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }

            glm::vec3 &position() { return mPosition; }
            void position(const glm::vec3 &newPosition) { mPosition = newPosition; }
            glm::vec4 &rotation() { return mRotation; }
            void rotation(const glm::vec4 &newRotation) { mRotation = newRotation; }
            glm::vec3 &scale() { return mScale; }
            void scale(const glm::vec3 &newScale) { mScale = newScale; }
            std::vector<Vertex> &vertices() { return mVertices; }
            void vertices(const std::vector<Vertex> &newVertices) { mVertices = newVertices; }
            void addVertex(const Vertex vertex) { mVertices.push_back(vertex); }

            //void setPosition
        protected:
            glm::vec3 mPosition;
            glm::vec4 mRotation;
            glm::vec3 mScale;

            std::vector<Vertex> mVertices;

            std::string mComponentName;
    };
}