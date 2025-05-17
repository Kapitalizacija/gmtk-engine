#pragma once

#include <vector>

#include <glm/glm.hpp>


namespace GMTKEngine {
    namespace Component {
        class Shape {
            public:
                Shape();
                Shape(std::vector<glm::vec2> vertices);

                std::vector<glm::vec2> getEdges();
                std::vector<glm::vec2> getNormals();
            private:
                std::vector<glm::vec2> vertices;
        };
    }
}