#pragma once

#include "../../component.hpp"

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

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }
            virtual bool changedSinceLastUpdate() override;  
            
            glm::vec3 getPosition();
            void setPosition(glm::vec3 newPos);

            glm::vec3 getScale();
            void setScale(glm::vec3 newScale);

            glm::vec4 getRotation();
            void setRotation(glm::vec4 newRotation);

            //void setPosition
        protected:
            bool changed;

            glm::vec3 mPosition;
            glm::vec4 mRotation;
            glm::vec3 mScale;

            std::vector<Vertex> mVertices;
    };
}