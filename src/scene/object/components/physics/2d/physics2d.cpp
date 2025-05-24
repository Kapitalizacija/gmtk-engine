#include "physics2d.hpp"

//The physics 2D engine hooks directly into the Transform2D component
namespace GMTKEngine {
    namespace Component {
        Physics2D::Physics2D(ResourceRef<Transform2D> transform, Shape shape, float mass, bool isSimulated): mMass(mass), mIsSimulated(isSimulated) {
            this->shape = std::make_shared<Shape>(shape);
            mForceDirection = glm::vec3(0.f, 0.f, 0.f);
            
        }

        void Physics2D::fixedUpdate() {
            float mul = 1 / FIXED_TIMESTEP_MS;
            if (mIsSimulated) {
                float Fg = mMass * (PhysicsConstants::g * mul);
                mForceDirection = glm::vec3(mForceDirection.x, mForceDirection.y + Fg, mForceDirection.z);
                
                glm::vec3 reducedVector = glm::vec3(mForceDirection * mul);
                reducedVector /= mMass; //Newton's 2nd law or something idk.
                glm::vec3 prevPos = transform->getPosition();
                transform->setPosition(prevPos + reducedVector);
            }
        }

        
        bool Physics2D::checkIntersection(ResourceRef<Physics2D> other) {
            ResourceRef<Shape> shape1 = shape;    
            ResourceRef<Shape> shape2 = other->getShape();    

            std::unordered_set<glm::vec2> perpendiculars;

            {
                std::unordered_set<glm::vec2> p1 = shape->getNormals();
                perpendiculars.insert(p1.begin(), p1.end());
            }

            {
                std::unordered_set<glm::vec2> p2 = shape->getNormals();
                perpendiculars.insert(p2.begin(), p2.end());
            }


            
            for(const glm::vec2& n : perpendiculars) {
                double min1, max1, min2, max2;
                min1 = min2 = std::numeric_limits<double>::max();
                max1 = max2 = std::numeric_limits<double>::min();
                
                for (const glm::vec2& v : shape1->getEdges()) {
                    float dotProduct = glm::dot(v, n);
 
                    if (dotProduct < min1) 
                        min1 = dotProduct;
                    else if(dotProduct > max1)
                        max1 = dotProduct;
                }

                for (const glm::vec2& v : shape2->getEdges()) {
                    float dotProduct = glm::dot(v, n);
 
                    if (dotProduct < min1) 
                        min2 = dotProduct;
                    else if(dotProduct > max1)
                        max2 = dotProduct;
                }   

                if ((min1 < max2 && min1 > min2) || (min2 < max1 && min2 > min1)) {
                    continue;
                }

                return false;
            }

            return true;
        }

        ResourceRef<Shape> Physics2D::getShape() {
            return (ResourceRef<Shape>)shape;
        }

        std::vector<size_t> Physics2D::getRequiredComponentHashes() const{
            return {
                typeid(Transform2D).hash_code()
            };
        }
    }
}