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
        class Transform2D : public Component {
            friend class Object;

            public:
                Transform2D(glm::vec3 position = glm::vec3(0.0f), float rotation = 0, float depth = 0,
                glm::vec3 scale = glm::vec3(1.0f));

                virtual void start() { return; }
                virtual void earlyUpdate(float dt) { return; }
                virtual void update(float dt) { return; }
                virtual void lateUpdate(float dt) { return; }
                virtual void frameCleanup() override;
                virtual void fixedUpdate() { return; }
                virtual bool hasChanged() override;  

                glm::vec3 getPosition();
                void setPosition(glm::vec3 newPos);
                void translate(glm::vec3 vec);

                glm::vec3 getScale();
                void setScale(glm::vec3 newScale);

                float getRotation();
                void setRotation(float newRotation);
                void rotate(float amount);

            protected:
                bool changed;

                glm::vec3 mPosition;
                glm::vec3 mScale;
                float rotation;

                std::string mComponentName;
        };
    }
}