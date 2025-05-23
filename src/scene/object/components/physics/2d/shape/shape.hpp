#pragma once

#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>

namespace std {
    template<>
    struct hash<glm::vec2> {
        auto operator()(const glm::vec2& ref) const -> size_t {
            return  (*(uint32_t*)&ref.x ^ 0x1f4bd2f4) ^ (*(uint32_t*)&ref.y ^ 0x1f4bd2f4);
        }
    };
}

namespace GMTKEngine {
    namespace Component {
        class Shape {
            public:
                Shape();
                Shape(std::vector<glm::vec2> vertices);

                std::unordered_set<glm::vec2> getEdges();
                std::unordered_set<glm::vec2> getNormals();
            private:
                std::vector<glm::vec2> vertices;
        };
    }
}
