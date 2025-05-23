#include "physics2d.hpp"

//The physics 2D engine hooks directly into the Transform2D component
namespace GMTKEngine {
    namespace Component {
        Physics2D::Physics2D(ResourceRef<Transform2D> transform, Shape shape, float mass): shape(std::shared_ptr<Shape>(shape)), mMass(mass) {
            mForceDirection = glm::vec3(0.f, 0.f, 0.f);
            
        }

        void Physics2D::fixedUpdate() {
            float Fg = mMass * (PhysicsConstants::g * (1 / FIXED_TIMESTEP_MS));
            mForceDirection = glm::vec3(mForceDirection.x, mForceDirection.y + Fg, mForceDirection.z);
        }

        
        void Physics2D::checkCollisionWithObject(ResourceRef<Physics2D> other) {
            
        }
    
        ResourceRef<Shape> Physics2D::getShape() {
            return shape;
        }
    }
}