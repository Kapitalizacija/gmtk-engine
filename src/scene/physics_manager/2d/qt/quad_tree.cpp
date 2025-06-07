#include "quad_tree.hpp"
                
namespace Sierra {

    QuadTree::QuadTree(glm::vec2 pos, glm::vec2 size): mRootNode() {
        mRootNode.pos = pos;
        mRootNode.size = size;
        mRootNode.parent = nullptr;
    }

    void QuadTree::update() {
        updateNode(mRootNode);
    }

    void QuadTree::addBody(ResourceRef<Component::Body2D> body) {
        addBodyToNode(mRootNode, body);
    }

    bool QuadTree::addBodyToNode(Node& node, ResourceRef<Component::Body2D> body) {
        if (!isInNode(node, body))
            return false;

        if (node.children.empty()) {
            node.bodies.insert(body);
            return true;
        }

        for (Node& child : node.children) {
            if (addBodyToNode(child, body))
                return true;
        }

        ERROR("Tried to add an out of bounds body to the quad tree");
        return false;
    }

    void QuadTree::updateNode(Node& node) { // recursion actually being useful in practical situations???? unheard of
        if (node.bodies.size() > NODE_OBJECT_TRESHOLD) {
            segmentNode(node);
        }

        for(Node& node : mRootNode.children) {
            updateNode(node);
        }

        for (auto it = node.bodies.begin(); it != node.bodies.end();) {
            if (!isInNode(node, *it)) {
                it = node.bodies.erase(it);
                addBody(*it);

                if (node.parent != nullptr)
                    node.parent->childBodyCount--;

                continue;
            }

            it++;
        }

        if (node.childBodyCount == 0) {
            node.children.clear();
        }
    }

    void QuadTree::segmentNode(Node& node) {
        if (!node.children.empty())
            return;

        node.children.resize(4);
            
        for (Node& child : node.children) {
            child.parent = &node;
            child.size = node.size / 2;
        }

        node.children[0].pos = glm::vec2(node.pos.x, node.pos.y);
        node.children[1].pos = glm::vec2(node.pos.x + node.size.x / 2, node.pos.y);
        node.children[2].pos = glm::vec2(node.pos.x, node.pos.y + node.size.y / 2);
        node.children[3].pos = glm::vec2(node.pos.x + node.size.x / 2, node.pos.y + node.size.y / 2);

        for (const ResourceRef<Component::Body2D>& body : node.bodies) {
            for (Node& child : node.children) {
                if (isInNode(child, body)) {
                    child.bodies.insert(body);
                    break;
                }
            }
        }

        node.childBodyCount = node.bodies.size();

        node.bodies.clear();
    }

    bool QuadTree::isInNode(Node& node, ResourceRef<Component::Body2D> body) {
        glm::vec2 bPos = body->mTransform->getPosition();

        if (bPos.x > node.pos.x && bPos.x < node.pos.x + node.size.x &&
            bPos.y > node.pos.y && bPos.y < node.pos.y + node.size.y) {
            return true;
        }

        return false;
    }
}