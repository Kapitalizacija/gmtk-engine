#pragma once

#include "util/utilities.hpp"
#include "io/logging/logger.hpp"

namespace Sierra {
    class DeltaTime {
        public:
            DeltaTime();

            void update();
            float get();
        private:
            float mDt;
            size_t mLast;

            const float MAX_DT = 0.32f; // to prevent game breaking stutters
    };
}