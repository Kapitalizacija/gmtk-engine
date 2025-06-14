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
                virtual void earlyUpdate(const StateInfo& stateInfo) { return; }
                virtual void update(const StateInfo& stateInfo) { return; }
                virtual void lateUpdate(const StateInfo& stateInfo) { return; }
                virtual void fixedUpdate(const StateInfo& stateInfo) { return; }
                virtual void frameCleanup() { return; }
                virtual bool hasChanged() { return false; }
            private:
                
        };
    }
}