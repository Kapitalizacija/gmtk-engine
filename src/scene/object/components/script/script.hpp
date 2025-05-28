#pragma once

#include <memory>
#include <unordered_map>
#include <cassert>
#include <limits>

#include "../component.hpp"
#include "util/utilities.hpp"

namespace Sierra {
    namespace Component {
        class Script : public Component {
            public:
                Script() = default;
                DISABLE_COPY_AND_MOVE(Script);

                virtual void start() { return; }
                virtual void earlyUpdate(float dt) { return; }
                virtual void update(float dt) { return; }
                virtual void lateUpdate(float dt) { return; }
                virtual void frameCleanup() { return; }
                virtual void fixedUpdate() { return; }
                virtual bool hasChanged() { return false; }
            private:
                
        };
    }
}