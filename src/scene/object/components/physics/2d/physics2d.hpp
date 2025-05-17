#pragma once

#include <memory>
#include <unordered_map>
#include "../../component.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "util/utilities.hpp"
#include "shape/shape.hpp"

namespace GMTKEngine {
    class Physics2D : public Component {
        public:
            Physics2D() = default;
            Physics2D(Shape shape);
            DISABLE_COPY_AND_MOVE(Physics2D);

            virtual void start() { return; }
            virtual void earlyUpdate() { return; }
            virtual void update() { return; }
            virtual void lateUpdate() { return; }
            virtual void frameCleanup() { return; }
            virtual void fixedUpdate() { return; }
            virtual bool hasChanged() { return false; }
        
        private:
            Shape shape;
    };
}