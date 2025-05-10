#pragma once

#include "../../component.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace GMTKEngine {

    class Transform2D : public Component {
        public:
            Transform2D(glm::vec2 position = glm::vec2(0.0f), float rotation = 0, float depth = 0,
             glm::vec2 scale = glm::vec2(1.0f));

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }
            virtual bool changedSinceLastUpdate() override;  
            
            glm::vec2 getPosition();
            void setPosition(glm::vec2 newPos);
            void translate(glm::vec2 vec);

            glm::vec2 getScale();
            void setScale(glm::vec2 newScale);

            float getRotation();
            void setRotation(float newRotation);
            void rotate(float amount);

            float getDepth();
            void setDepth(float newDepth);

        protected:
            bool changed;

            glm::vec2 mPosition;
            glm::vec2 mScale;
            float depth;
            float rotation;

            std::string mComponentName;
    };
}