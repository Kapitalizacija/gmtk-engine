#include <glm/glm.hpp>
#include "util/utilities.hpp"

namespace Sierra::Component {
    class PhysicsConstants {
        public:
            PhysicsConstants() = delete;
            DISABLE_COPY_AND_MOVE(PhysicsConstants);

            static constexpr const float g = 9.18f;
            static constexpr const int c = 297792548;
    };
}