#include "physics_manager2d.hpp"

namespace Sierra {
    PhysicsManager2D::PhysicsManager2D() {
        physicsConstants = std::make_shared<PhysicsConstants>();
    }

    void PhysicsManager2D::addBody(ResourceRef<Component::Body2D> body) {
        if (body.isEmpty()) {
            ERROR("Tried to add null body to the physics manager");
            return;
        }

        body->setPhysicsConstants(physicsConstants);

        bodies.insert(body);
    }

    void PhysicsManager2D::removeBody(ResourceRef<Component::Body2D> body) {
        bodies.erase(body);
    }

    void PhysicsManager2D::update() {
        for (ResourceRef<Component::Body2D> b1 : bodies) { // TODO 
            for (ResourceRef<Component::Body2D> b2 : bodies) {
                if (b1 == b2) {
                    continue;
                }

                b1->resolveCollision(b2); 
            }
        }
    }

    void PhysicsManager2D::setConstants(PhysicsConstants& constants) {
        *physicsConstants = constants;
    }

    PhysicsConstants PhysicsManager2D::getConstants() {
        return *physicsConstants;
    }
}