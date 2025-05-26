#pragma once

#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

#include "io/logging/logger.hpp"

namespace std {
    template<>
    struct hash<glm::vec2> {
        auto operator()(const glm::vec2& ref) const -> size_t {
            size_t h1 = (*(uint32_t*)&ref.x ^ 0x9e3779b9);
            size_t h2 = (*(uint32_t*)&ref.y ^ 0x9e3779b9);

            h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);

            return h1;
        }
    };
}

namespace GMTKEngine {
    namespace Component {
        class Shape {
            public:
                Shape();
                Shape(std::vector<glm::vec2> vertices);

                std::vector<glm::vec2> getNormals();
                std::vector<glm::vec2> getVertices();
                std::vector<glm::vec2> getRotatedVertices(float angle);
                std::vector<glm::vec2> getRotatedNormals(float angle);

                glm::vec2 getCenterOfGeometry();

                private:

                void normalizeVertices(std::vector<glm::vec2> vertices);
                std::vector<glm::vec2> calcNormals(std::vector<glm::vec2> vertices);
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
