#include "physics2d.hpp"

//The physics 2D engine hooks directly into the Transform2D component
namespace Sierra {
    namespace Component {

        Physics2D::Physics2D(): mMass(1), mForceDirection(), mIsSimulated(true) {
            this->mShape = std::make_shared<Shape>();
        }

        Physics2D::Physics2D(ResourceRef<Transform2D> transform, Shape shape, float mass, bool isSimulated): mMass(mass), mIsSimulated(isSimulated) {
            this->mShape = std::make_shared<Shape>(shape);
            mForceDirection = glm::vec3(0.f, 0.f, 0.f);
        }

        void Physics2D::fixedUpdate() {
            float mul = 1 / FIXED_TIMESTEP_MS;
            if (mIsSimulated) {
                float Fg = mMass * (PhysicsConstants::g * mul);
                mForceDirection = glm::vec3(mForceDirection.x, mForceDirection.y + Fg, mForceDirection.z);
                
                glm::vec3 reducedVector = glm::vec3(mForceDirection * mul);
                reducedVector /= mMass; //Newton's 2nd law or something idk.
                glm::vec3 prevPos = mTransform->getPosition();
                mTransform->setPosition(prevPos + reducedVector);
            }
        }

        
        glm::vec2 Physics2D::checkIntersection(ResourceRef<Physics2D> other) {
            ResourceRef<Shape> shape1 = mShape;    
            ResourceRef<Shape> shape2 = other->getShape();    

            std::unordered_set<glm::vec2> perpendiculars;

            {
                std::vector<glm::vec2> p1 = mShape->getRotatedNormals(mTransform->getRotation());
                perpendiculars.insert(p1.begin(), p1.end());
            }

            {
                std::vector<glm::vec2> p2 = mShape->getRotatedNormals(other->mTransform->getRotation());
                perpendiculars.insert(p2.begin(), p2.end());
            }

            glm::vec2 resolveAxis = glm::vec2(0.0);
            float minOverlap = std::numeric_limits<float>::max();

            for(const glm::vec2& n : perpendiculars) {
                double min1, max1, min2, max2;
                min1 = min2 = std::numeric_limits<double>::max();
                max1 = max2 = std::numeric_limits<double>::min();
                
                for (const glm::vec2& v : shape1->getRotatedVertices(mTransform->getRotation())) {
                    glm::vec2 p = v * (glm::vec2)mTransform->getScale() + (glm::vec2)mTransform->getPosition();
                    float dotProduct = glm::dot(p, n);

                    if (dotProduct < min1) 
                        min1 = dotProduct;
                    if(dotProduct > max1)
                        max1 = dotProduct;
                }

                for (const glm::vec2& v : shape2->getRotatedVertices(other->mTransform->getRotation())) {
                    glm::vec2 p = v * (glm::vec2)other->mTransform->getScale() + (glm::vec2)other->mTransform->getPosition();
                    float dotProduct = glm::dot(p, n);
 
                    if (dotProduct < min2) 
                        min2 = dotProduct;
                    if(dotProduct > max2)
                        max2 = dotProduct;
                }   

                float overlap = std::min(
                    std::abs(min1 - max2),
                    std::abs(min2 - max1)
                );

                if (overlap < minOverlap){
                    minOverlap = overlap;
                    resolveAxis = n;
                }

                if ((min1 <= max2 && min1 >= min2) || (min2 <= max1 && min2 >= min1)) {
                    continue;
                }

                return glm::vec2(0.0);
            }

            return resolveAxis * minOverlap;
        }
        
        bool Physics2D::resolveCollision(ResourceRef<Physics2D> other) {
            if (!mIsSimulated && other->mIsSimulated)
                return false;
            
            glm::vec2 mtv = checkIntersection(other);

            if ((!mtv.x && !mtv.y))
                return false;

            if (!mIsSimulated && other->mIsSimulated) {
                other->mTransform->translate(glm::vec3(mtv, 0.0));
                return true;
            } else if (mIsSimulated && !other->mIsSimulated) {
                mTransform->translate(glm::vec3(-mtv, 0.0));
                return true;
            }


            float selfPush = other->getMass() / (mMass + other->getMass());
            float otherPush = mMass / (mMass + other->getMass());

            mTransform->translate(glm::vec3(-mtv * selfPush, 0.0));
            other->mTransform->translate(glm::vec3(mtv * otherPush, 0.0));

            return true;
        }

        ResourceRef<Shape> Physics2D::getShape() {
            return (ResourceRef<Shape>)mShape;
        }

        float Physics2D::getMass() {
            return mMass;
        }

        std::vector<size_t> Physics2D::getRequiredComponentHashes() const{
            return {
                typeid(Transform2D).hash_code()
            };
        }

        void Physics2D::setRequiredComponents(std::vector<ResourceRef<Component>> components) {
            assert(components.size() == 1);

            mTransform = components[0];
        }
    }
}