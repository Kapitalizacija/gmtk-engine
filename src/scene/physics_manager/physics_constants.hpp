#pragma once

#include <glm/glm.hpp>
#include "util/utilities.hpp"

namespace Sierra{
    struct PhysicsConstants {
        glm::vec3 g = glm::vec3(0.0f, -9.18f, 0.0f);
        float air_drag = 0.2f; //idk physics -C * v2
        int c = 297792548;
    };
}