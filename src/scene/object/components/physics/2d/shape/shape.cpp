#include "shape.hpp"

namespace GMTKEngine {
    namespace Component {
        Shape::Shape() { 
            mVertices = {
                {0.0f, 0.0f},
                {0.0f, 100.0f},
                {100.0f, 0.0f},
                {100.0f, 100.0f}
            };

            calcNormals();
        ;}    

        Shape::Shape(std::vector<glm::vec2> vertices): mVertices(vertices) {
            calcNormals();
        }

        void Shape::calcNormals() {

            std::unordered_set<glm::vec2> edges;
            edges.reserve(mVertices.size());

            for (int i = 0; i < mVertices.size(); i++) {
                edges.insert(
                    mVertices[i] - mVertices[(i + 1) % mVertices.size()]
                );
            }

            std::unordered_set<glm::vec2> normals;
            normals.reserve(edges.size());

            for(glm::vec2 edge : edges) {
                normals.insert({
                    -edge.y,
                     edge.x
                    }
                );
            }

            mNormalizedNormals.reserve(normals.size());

            for (const glm::vec2& normal : normals) {
                mNormalizedNormals.push_back(glm::normalize(normal));
            }
        }

        std::vector<glm::vec2> Shape::getVertices() {
            return mVertices;
        }

        std::vector<glm::vec2> Shape::getNormals() {
            return mNormalizedNormals;
        }
    }
}