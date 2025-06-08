#include "body2d.hpp"

// The physics 2D engine hooks directly into the Transform2D component
namespace Sierra
{
    namespace Component
    {

        Body2D::Body2D() : mInfo(), mVel()
        {
            this->mShape = std::make_shared<Shape>();
        }

        Body2D::Body2D(Info &info) : mInfo(info), mVel()
        {
            this->mShape = std::make_shared<Shape>();
        }

        void Body2D::fixedUpdate()
        {
            if (physicsConstants.isEmpty())
            {
                ERROR("Physics constants not set in object, cannot update.");
                return;
            }

            float mul = FIXED_TIMESTEP_MS / 1000.0f;
            if (!mIsSimulated)
            {
                return;
            }

            glm::vec3 dragForce = glm::vec3(0.0f);
            if (!physicsConstants->top_down_physics) {
                dragForce = mVel * physicsConstants->airDrag * mul;
            }
            else {
                dragForce = glm::sign(mVel) * physicsConstants->airDrag * mul;
            }
            mVel -= dragForce;

            if (misAffectedByGravity)
            {
                glm::vec3 Fg = physicsConstants->g * mul;
                mVel += Fg;
            }
        }

        void Body2D::lateUpdate(float dt)
        {
            mTransform->translate(mVel * dt);

            if (std::abs(mVel.x) < 1 && std::abs(mVel.y) < 1)
            {
                mIsResting = true;
                mVel = glm::vec3(0.0);
            }
        }

        glm::vec2 Body2D::checkIntersection(ResourceRef<Body2D> other)
        {
            ResourceRef<Shape> shape1 = mShape;
            ResourceRef<Shape> shape2 = other->getShape();

            std::unordered_set<glm::vec2> perpendiculars;

            {
                std::vector<glm::vec2> p1 = mShape->getNormals(mTransform->getRotation());
                perpendiculars.insert(p1.begin(), p1.end());
            }

            {
                std::vector<glm::vec2> p2 = mShape->getNormals(other->mTransform->getRotation());
                perpendiculars.insert(p2.begin(), p2.end());
            }

            glm::vec2 resolveAxis = glm::vec2(0.0);
            float minOverlap = std::numeric_limits<float>::max();

            for (const glm::vec2 &n : perpendiculars)
            {
                double min1, max1, min2, max2;
                min1 = min2 = std::numeric_limits<double>::max();
                max1 = max2 = std::numeric_limits<double>::min();

                for (const glm::vec2 &v : shape1->getVertices(mTransform->getRotation(), mTransform->getScale()))
                {
                    glm::vec2 p = v + (glm::vec2)mTransform->getPosition();
                    float dotProduct = glm::dot(p, n);

                    if (dotProduct < min1)
                        min1 = dotProduct;
                    if (dotProduct > max1)
                        max1 = dotProduct;
                }

                for (const glm::vec2 &v : shape2->getVertices(other->mTransform->getRotation(), other->mTransform->getScale()))
                {
                    glm::vec2 p = v + (glm::vec2)other->mTransform->getPosition();
                    float dotProduct = glm::dot(p, n);

                    if (dotProduct < min2)
                        min2 = dotProduct;
                    if (dotProduct > max2)
                        max2 = dotProduct;
                }

                float overlap = std::min(
                    std::abs(min1 - max2),
                    std::abs(min2 - max1));

                if (overlap < minOverlap)
                {
                    minOverlap = overlap;
                    resolveAxis = n;
                }

                if ((min1 <= max2 && min1 >= min2) || (min2 <= max1 && min2 >= min1))
                {
                    continue;
                }

                return glm::vec2(0.0);
            }

            resolveAxis = glm::abs(resolveAxis);

            if (mTransform->getPosition().y > other->mTransform->getPosition().y)
            {
                resolveAxis.y *= -1;
            }

            if (mTransform->getPosition().x > other->mTransform->getPosition().x)
            {
                resolveAxis.x *= -1;
            }

            return resolveAxis * minOverlap;
        }

        bool Body2D::resolveCollision(ResourceRef<Body2D> other)
        {
            if (!mIsSimulated && other->mIsSimulated)
                return false;

            glm::vec2 mtv = checkIntersection(other);

            if ((!mtv.x && !mtv.y))
                return false;

            glm::vec2 normal1 = getIntersectingNormal(mShape->getVertices(mTransform->getRotation(), mTransform->getScale()), glm::normalize(mtv));
            glm::vec2 normal2 = getIntersectingNormal(other->mShape->getVertices(other->mTransform->getRotation(), other->mTransform->getScale()), mtv);

            glm::vec2 normalizedMTV = glm::normalize(mtv);
            glm::vec2 normalizedG = glm::normalize(physicsConstants->g);
            if (normalizedMTV == normalizedG)
            {
                mIsGrounded = true;
            }
            else if (normalizedMTV == -normalizedG)
            {
                other->mIsGrounded = true;
            }

            if (!mIsSimulated && other->mIsSimulated)
            {
                other->mTransform->translate(glm::vec3(mtv, 0.0));
                other->mVel *= glm::abs(glm::vec3(normal1.y, normal1.x, 1.0f));
                return true;
            }
            if (mIsSimulated && !other->mIsSimulated)
            {
                mTransform->translate(glm::vec3(-mtv, 0.0));
                mVel *= glm::abs(glm::vec3(normal2.y, normal2.x, 1.0f));

                return true;
            }

            mtv *= 1.01;

            // just resolution
            float selfPush = other->mInfo.mass / (mInfo.mass + other->mInfo.mass);
            float otherPush = mInfo.mass / (mInfo.mass + other->mInfo.mass);

            mTransform->translate(glm::vec3(-mtv * selfPush, 0.0));
            other->mTransform->translate(glm::vec3(mtv * otherPush, 0.0));
            //

            glm::vec2 v1 = mVel;
            glm::vec2 v2 = other->mVel;

            float cr = std::min(mInfo.elasticity, other->mInfo.elasticity);

            mVel = glm::vec3(
                (cr * other->mInfo.mass * (v2 - v1) + mInfo.mass * v1 + other->mInfo.mass * v2) / (mInfo.mass + other->mInfo.mass),
                0.0);

            other->mVel = glm::vec3(
                (cr * mInfo.mass * (v1 - v2) + mInfo.mass * v1 + other->mInfo.mass * v2) / (mInfo.mass + other->mInfo.mass),
                0.0);

            return true;
        }

        glm::vec2 Body2D::getIntersectingNormal(std::vector<glm::vec2> vertices, glm::vec2 p)
        {
            float minDiff = std::numeric_limits<float>::max();
            glm::vec2 minEdge = glm::vec2(0.0f);

            for (int i = 0; i < vertices.size() - 1; i++)
            {
                glm::vec2 a = vertices[i];
                glm::vec2 b = vertices[(i + 1) % vertices.size()];

                float totalDist = glm::length(a - b);
                float distA = glm::length(a - p);
                float distB = glm::length(b - p);

                float diff = std::abs(totalDist - (distA + distB));

                if (diff < minDiff)
                {
                    glm::vec2 edge = a - b;

                    minEdge = glm::normalize(glm::vec2(-edge.y, edge.x));
                    minDiff = diff;
                }
            }

            return minEdge;
        }

        void Body2D::applyImpulse(glm::vec3 f)
        {
            mVel += f / mInfo.mass;
        }

        ResourceRef<Shape> Body2D::getShape()
        {
            return (ResourceRef<Shape>)mShape;
        }

        void Body2D::setPhysicsConstants(ResourceRef<PhysicsConstants> constants)
        {
            physicsConstants = constants;
        }

        Body2D::Info Body2D::getInfo()
        {
            return mInfo;
        }

        void Body2D::setInfo(Info info)
        {
            mInfo = info;
        }

        bool Body2D::getIsSimulated()
        {
            return mIsSimulated;
        }

        void Body2D::setIsSimulated(bool isSimulated)
        {
            mIsSimulated = isSimulated;
        }

        bool Body2D::getIsAffectedByGravity()
        {
            return misAffectedByGravity;
        }

        void Body2D::setIsAffectedByGravity(bool isAffectedByGravity)
        {
            misAffectedByGravity = isAffectedByGravity;
        }

        std::vector<size_t> Body2D::getRequiredComponentHashes() const
        {
            return {typeid(Transform2D).hash_code()};
        }

        void Body2D::setRequiredComponents(std::vector<ResourceRef<Component>> components)
        {
            assert(components.size() == 1);

            mTransform = components[0];
        }

    }
}