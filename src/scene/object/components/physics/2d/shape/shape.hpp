#pragma once

#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

#include "io/logging/logger.hpp"

namespace std {
    template<>
    struct hash<glm::vec2> {
        auto operator()(const glm::vec2& ref) const -> size_t {
            uint32_t h1 = (*(uint32_t*)&ref.x ^ 0x9e3779b9);
            uint32_t h2 = (*(uint32_t*)&ref.y ^ 0x9e3779b9);

 //           h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);

            size_t combination = h1 + ((size_t)h2 << 32);

            return combination;
        }
    };
}

namespace Sierra {
    namespace Component {
        class Shape {
            public:
                Shape();
                Shape(std::vector<glm::vec2> vertices);

                std::vector<glm::vec2> getVertices(float angle, glm::vec2 scale = glm::vec2(1.0f));
                std::vector<glm::vec2> getEdges(float angle);
                std::vector<glm::vec2> getNormals(float angle);

                glm::vec2 getCenterOfGeometry();

                private:

                void normalizeVertices(std::vector<glm::vec2> vertices);
                void calcSignedArea();
                void calcCenterOfGeometry();
                void calcVertDistFromCenter();

                std::vector<glm::vec2> mVertices;
                std::vector<glm::vec2> mNormalizedNormals;
                std::vector<std::pair<float, float>> mDistVertFromCenter; // magnitude and angle relative to the geom center

                float mSignedArea;
                glm::vec2 mCenterOfGeometry;
        };
    }
}
