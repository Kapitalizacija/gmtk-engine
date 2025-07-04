#include "shape.hpp"

namespace Sierra {
    namespace Component {
        Shape::Shape() { 
           std::vector<glm::vec2> vertices = {
                {0.0f, 0.0f},
                {0.0f, 1.0f},
                {1.0f, 1.0f},
                {1.0f, 0.0f},
            }; // MUST ALWAYS BE ORDERED

            normalizeVertices(vertices);

            mNormalizedNormals = getNormals(0);

            calcSignedArea();
            calcCenterOfGeometry();
            calcVertDistFromCenter();

            for(glm::vec2& v : mVertices) {
                v -= mCenterOfGeometry;
            }
        ;}    

        Shape::Shape(std::vector<glm::vec2> vertices) {
            normalizeVertices(vertices);
            mNormalizedNormals = getNormals(0);
            calcSignedArea();
            calcCenterOfGeometry();
            calcVertDistFromCenter();
        }

        void Shape::normalizeVertices(std::vector<glm::vec2> vertices) {
            glm::vec2 minOffset = glm::vec2(std::numeric_limits<float>::max());

            mVertices = std::move(vertices);

            for (const glm::vec2& v : mVertices) {
                minOffset.x = std::min(v.x, minOffset.x);
                minOffset.y = std::min(v.y, minOffset.y);
            }
        }

        std::vector<glm::vec2> Shape::getEdges(float angle) {
            std::vector<glm::vec2> vertices = getVertices(angle);

            std::unordered_set<glm::vec2> edges;
            edges.reserve(vertices.size());

            for (int i = 0; i < vertices.size(); i++) {
                edges.insert(
                    vertices[i] - vertices[(i + 1) % vertices.size()]
                );
            }

            std::vector<glm::vec2> vEdges;
            vEdges.insert(vEdges.end(), edges.begin(), edges.end());

            return vEdges;
        }

        std::vector<glm::vec2> Shape::getNormals(float angle) {
            std::vector<glm::vec2> edges = getEdges(angle);

            std::vector<glm::vec2> normals;
            normals.reserve(edges.size());

            for(glm::vec2 edge : edges) {
                normals.push_back({
                    glm::normalize(glm::vec2(
                    -edge.y,
                     edge.x))
                    }
                );
            }

            return normals;
        }

        void Shape::calcSignedArea() {
            mSignedArea = 0;
            for (int i = 0; i < mVertices.size() - 1; i++) {
                mSignedArea += mVertices[i].x * mVertices[i+1].y - mVertices[i+1].x * mVertices[i].y;
            }

            mSignedArea /= 2;
        }

        void Shape::calcCenterOfGeometry() {
            mCenterOfGeometry = glm::vec2(0.0);

            for (int i = 0; i < mVertices.size() - 1; i++) {
                mCenterOfGeometry.x += (mVertices[i].x + mVertices[i+1].x) * (mVertices[i].x * mVertices[i+1].y - mVertices[i+1].x * mVertices[i].y);
                mCenterOfGeometry.y += (mVertices[i].y + mVertices[i+1].y) * (mVertices[i].x * mVertices[i+1].y - mVertices[i+1].x * mVertices[i].y);
            }

            mCenterOfGeometry /= mSignedArea * 6;
        }

        void Shape::calcVertDistFromCenter() {
            mDistVertFromCenter.reserve(mVertices.size());

            for (const glm::vec2& v : mVertices) {
                mDistVertFromCenter.emplace_back(
                    glm::length(v - mCenterOfGeometry),
                    std::atan2(v.y - mCenterOfGeometry.y, v.x - mCenterOfGeometry.x)
                );
            }
        }

        std::vector<glm::vec2> Shape::getVertices(float angle, glm::vec2 scale) {

            std::vector<glm::vec2> rotatedVertices;
            rotatedVertices.reserve(mVertices.size());

            if (angle == 0) {
                for (const glm::vec2& v : mVertices) {
                    rotatedVertices.emplace_back(
                        v * scale
                    );
                }

                return rotatedVertices;
            }

            if (scale.x == scale.y) { // faster because precomputed
                for (int i = 0; i < mDistVertFromCenter.size(); i++) {
                    rotatedVertices.emplace_back(
                        cos(angle + mDistVertFromCenter[i].second) * mDistVertFromCenter[i].first * scale.x, 
                        sin(angle + mDistVertFromCenter[i].second) * mDistVertFromCenter[i].first * scale.y
                    );
                }
                return rotatedVertices;
            }

            glm::mat2 rotationMatrix = {
                cos(angle), -sin(angle),
                sin(angle),  cos(angle)
            };

            for (const glm::vec2& v : mVertices) {
                rotatedVertices.emplace_back(
                    rotationMatrix * (v * scale)
                );
            }

            return rotatedVertices;

            //maxOffset -= minOffset;

        }

        glm::vec2 Shape::getCenterOfGeometry() {
            return mCenterOfGeometry;
        }
    }
}