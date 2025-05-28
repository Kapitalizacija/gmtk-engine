#pragma once

#include "../../component.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Sierra {
    namespace Component {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
        };

        class Transform3D : public Component {
            friend class Object;

            public:
                Transform3D(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec4 rotation = glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(0.f, 0.f, 0.f), std::vector<Vertex> vertices = {});

                virtual void start() { return; }
                virtual void earlyUpdate(float dt) { return; }
                virtual void update(float dt) { return; }
                virtual void lateUpdate(float dt) { return; }
                virtual void frameCleanup() override;
                virtual void fixedUpdate() { return; }
                virtual bool hasChanged() override;  

                glm::vec3 getPosition();
                void setPosition(glm::vec3 newPos);

                glm::vec3 getScale();
                void setScale(glm::vec3 newScale);

                glm::vec4 getRotation();
                void setRotation(glm::vec4 newRotation);

            protected:
                bool changed;

                glm::vec3 mPosition;
                glm::vec4 mRotation;
                glm::vec3 mScale;

                std::vector<Vertex> mVertices;
        };
    }
}