#pragma once

#include "../../component.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace GMTKEngine {

    class Transform2D : public Component {
        public:
            Transform2D(glm::vec2 position = glm::vec3(0.f, 0.f, 0.f), float rotation = 0,
             glm::vec2 scale = glm::vec3(0.f, 0.f, 0.f));
            ~Transform2D();

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }

            
            glm::vec2 getPosition();
            void setPosition(glm::vec2 newPos);

            glm::vec2 getScale();
            void setScale(glm::vec2 newScale);

            float getRotation();
            void setRotation(float newRotation);

            //void setPosition
        protected:
            glm::vec2 mPosition;
            glm::vec2 mScale;
            float depth;
            float rotation;

            std::string mComponentName;
    };
}