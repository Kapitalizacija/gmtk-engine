#pragma once

#include <memory>
#include <unordered_map>
#include <cassert>
#include <limits>

#include <GLFW/glfw3.h>

#include "../../component.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "util/utilities.hpp"
#include "shape/shape.hpp"
#include "scene/object/components/transform/2d/transform2d.hpp"
#include "scene/physics_manager/physics_constants.hpp"

namespace Sierra {
    namespace Component {

        class Body2D : public Component {
            friend class PhysicsManager2D;

            public:
                Body2D();
                Body2D(ResourceRef<Transform2D> transform, Shape shape, float mass = 1.f, bool isSimulated = true);
                DISABLE_COPY_AND_MOVE(Body2D);

                glm::vec2 checkIntersection(ResourceRef<Body2D> other);
                bool resolveCollision(ResourceRef<Body2D> other);

                ResourceRef<Shape> getShape();
                float getMass();

                void setPhysicsConstants(ResourceRef<PhysicsConstants> constants);
                
                virtual void start() { return; }
                virtual void earlyUpdate(float dt) { return; }
                virtual void update(float dt) { return; }
                virtual void lateUpdate(float dt) { return; }
                virtual void frameCleanup() { return; }
                virtual void fixedUpdate();
                virtual bool hasChanged() { return false; }
            
                virtual std::vector<size_t> getRequiredComponentHashes() const override;
                virtual void setRequiredComponents(std::vector<ResourceRef<Component>> components) override;
            protected:

            private:
            bool mIsSimulated;
            float mMass;
            
            std::shared_ptr<Shape> mShape;
            
            glm::vec3 mVel;
            
            ResourceRef<Transform2D> mTransform;
            ResourceRef<PhysicsConstants> physicsConstants;
        };
    }
}