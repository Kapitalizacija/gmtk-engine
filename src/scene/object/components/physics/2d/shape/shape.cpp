#include "shape.hpp"

namespace GMTKEngine {
    namespace Component {
        Shape::Shape() { 
            vertices = {
                {0.0f, 0.0f},
                {0.0f, 1.0f},
                {1.0f, 0.0f},
                {1.0f, 1.0f}
            };
        }    

        Shape::Shape(std::vector<glm::vec2> vertices): vertices(vertices) {

        }

    
        std::unordered_set<glm::vec2> Shape::getEdges() {
            std::unordered_set<glm::vec2> edges;
            edges.reserve(vertices.size());

            for (int i = 0; i < vertices.size(); i++) {
                edges.insert(
                    vertices[i] - vertices[(i + 1) % vertices.size()]
                );
            }

            return edges;
        }
    
        std::unordered_set<glm::vec2> Shape::getNormals() {
            std::unordered_set<glm::vec2> edges = getEdges();

            std::unordered_set<glm::vec2> normals;
            normals.reserve(edges.size());

            for(glm::vec2 edge : edges) {
                normals.insert({
                    -edge.y,
                     edge.x
                    }
                );
            }

            return normals;
        }
    }
}