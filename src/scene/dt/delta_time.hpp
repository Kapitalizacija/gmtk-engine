#pragma once

#include "util/utilities.hpp"

namespace Sierra {
    class DelaTime {
        public:
            void update();
            float get();
        private:
            float mDt;
            float mLast;

            const float MAX_DT = 0.032f; // to prevent game breaking stutters
    };
}