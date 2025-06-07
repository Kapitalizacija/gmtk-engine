#pragma once

#include <vector>
#include <unordered_set>

#include "scene/object/components/physics/2d/body2d.hpp"

namespace Sierra {
    class QuadTree {
        struct Node {
            std::unordered_set<ResourceRef<Component::Body2D>> bodies;
            std::vector<Node> children;
            uint32_t childBodyCount;

            Node* parent;

            glm::vec2 pos;
            glm::vec2 size;
        };

        public:
            QuadTree(glm::vec2 pos, glm::vec2 size);

            QuadTree(const QuadTree&) = delete;
            QuadTree(QuadTree&&) = delete;

            void update();
            void draw();

            void addBody(ResourceRef<Component::Body2D> body);

        private:
            void updateNode(Node& node);
            void segmentNode(Node& node);

            bool isInNode(Node& node, ResourceRef<Component::Body2D> body);
            bool addBodyToNode(Node& node, ResourceRef<Component::Body2D> body);

            Node mRootNode;

            const size_t NODE_OBJECT_TRESHOLD = 10;
    };
}