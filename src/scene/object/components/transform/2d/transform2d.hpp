#pragma once

#include "../../component.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace GMTKEngine {

    class Transform2D : public Component {
        public:
            Transform2D(glm::vec3 position = glm::vec3(0.0f), float rotation = 0, float depth = 0,
            glm::vec3 scale = glm::vec3(1.0f));

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }
            virtual void frameCleanup() override;
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