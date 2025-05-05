#pragma once

#include <glm/glm.hpp>
#include <AL/al.h>

namespace GMTKEngine {
    class ALListener {
        public:
            static void setPosition(const glm::vec3 &pos);
            static void setVelocity(const glm::vec3 &vel);
            static void setOrientation(const glm::vec3 &at, const glm::vec3 &up);
    };
}