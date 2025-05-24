#pragma once

#include <memory>
#include <unordered_map>
#include <cassert>
#include <limits>

#include "../../component.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "util/utilities.hpp"
#include "shape/shape.hpp"
#include "../physicsconstants.hpp"
#include "scene/object/components/transform/2d/transform2d.hpp"

namespace GMTKEngine {

    namespace Component {

        class Physics2D : public Component {
            public:
                Physics2D() = default;
                Physics2D(ResourceRef<Transform2D> transform, Shape shape, float mass = 1.f, bool isSimulated = true);
                DISABLE_COPY_AND_MOVE(Physics2D);

                bool checkIntersection(ResourceRef<Physics2D> other);
                ResourceRef<Shape> getShape();
                
                virtual void start() { return; }
                virtual void earlyUpdate() { return; }
                virtual void update() { return; }
                virtual void lateUpdate() { return; }
                virtual void frameCleanup() { return; }
                virtual void fixedUpdate();
                virtual bool hasChanged() { return false; }
            
            protected:
                virtual std::vector<size_t> getRequiredComponentHashes() const override;
                virtual void setRequiredComponents(std::vector<ResourceRef<Component>> components) override;
                

            private:
                bool mIsSimulated;
                std::shared_ptr<Shape> shape;
                glm::vec3 mForceDirection;
                float mMass;

                ResourceRef<Transform2D> transform;
        };
    }
}