#include "quad_tree.hpp"
                
namespace Sierra {

    QuadTree::QuadTree(glm::vec2 pos, glm::vec2 size): mRootNode() {
        mRootNode.pos = pos;
        mRootNode.size = size;
        mRootNode.depth = 1;
    }

    void QuadTree::update() {
        updateNode(mRootNode);
        resolveCollisionsOnNode(mRootNode);
    }

    void QuadTree::draw() {
        drawNode(mRootNode);
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
            
        bool inNode = false;
        for (Node& child : node.children) {
            if (addBodyToNode(child, body))
                inNode = true;
        }

        return inNode;
    }

    void QuadTree::updateNode(Node& node) { // recursion actually being usefu???? unheard of
        if (node.bodies.size() > NODE_OBJECT_TRESHOLD && node.depth <= MAX_DEPTH) {
            segmentNode(node);
        }
        
        for(Node& child : node.children) {
            updateNode(child);
        }


        for (auto it = node.bodies.begin(); it != node.bodies.end();) {
            if (isInNode(node, *it)) {
                it++;
                continue;
            }

            addBody(*it);
            it = node.bodies.erase(it);
        }

        bool childrenEmpty = true;
        for(Node& child : node.children) {
            if (!child.bodies.empty() || !child.children.empty()) {
                childrenEmpty = false;
                break;
            }
        }

        if (childrenEmpty)
            node.children.clear();
    }

    void QuadTree::segmentNode(Node& node) {
        if (!node.children.empty())
            return;

        node.children.resize(4);
            
        for (Node& child : node.children) {
            child.size = node.size / 2;
            child.depth = node.depth + 1;
        }

        node.children[0].pos = glm::vec2(node.pos.x - node.size.x / 4, node.pos.y - node.size.y / 4);
        node.children[1].pos = glm::vec2(node.pos.x + node.size.x / 4, node.pos.y - node.size.x / 4);
        node.children[2].pos = glm::vec2(node.pos.x - node.size.x / 4, node.pos.y + node.size.y / 4);
        node.children[3].pos = glm::vec2(node.pos.x + node.size.x / 4, node.pos.y + node.size.y / 4);

        for (const ResourceRef<Component::Body2D>& body : node.bodies) {
            addBody(body);
        }

        node.bodies.clear();
    }
    
    void QuadTree::drawNode(const Node& node) {
        Debug::Renderer::drawRect(glm::vec3(node.pos, 0.0), node.size, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), false, true);

        for (const Node& child : node.children) {
            drawNode(child);
        }
    }

    bool QuadTree::isInNode(Node& node, ResourceRef<Component::Body2D> body) {
        glm::vec2 bSize = body->mTransform->getScale();
        glm::vec2 bPos = (glm::vec2)body->mTransform->getPosition();

        if (
           bPos.x + bSize.x / 2 > node.pos.x - node.size.x / 2 && node.pos.x + node.size.x / 2 > bPos.x - bSize.x / 2 &&
           bPos.y + bSize.y / 2 > node.pos.y - node.size.y / 2 && node.pos.y + node.size.y / 2 > bPos.y - bSize.y / 2 
        ) {
            return true;
        }

        return false;
    }

    void QuadTree::resolveCollisionsOnNode(Node& node) {
        if (!node.children.empty()) {
            for (Node& child : node.children) {
                resolveCollisionsOnNode(child);
            }
        }

        for(ResourceRef<Component::Body2D> b1 : node.bodies) {
            for(ResourceRef<Component::Body2D> b2 : node.bodies) {
                if (b1 == b2)
                    continue;

                b1->resolveCollision(b2);
            }
        }
    }
}