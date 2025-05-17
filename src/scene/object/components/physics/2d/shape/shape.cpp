#include "shape.hpp"

namespace GMTKEngine {
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

    
    std::vector<glm::vec2> Shape::getEdges() {
        std::vector<glm::vec2> edges;
        edges.reserve(vertices.size());

        for (int i = 0; i < vertices.size(); i++) {
            edges.push_back(
                vertices[i] - vertices[(i + 1) % vertices.size()]
            );
        }

        return edges;
    }
    
    std::vector<glm::vec2> Shape::getNormals() {
        std::vector<glm::vec2> edges = getEdges();

        std::vector<glm::vec2> normals;
        normals.reserve(edges.size());

        for (int i = 0; i < vertices.size(); i++) {
            normals.push_back({
                -edges[i].y,
                 edges[i].x
                }
            );
        }

        return normals;
    }
}