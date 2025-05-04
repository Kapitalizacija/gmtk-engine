#pragma once

#include <string>
#include "io/logging/logger.hpp"

namespace GMTKEngine {
    class Component {
        friend class Object;

        protected:
            virtual void start() = 0;
            virtual void early_update() = 0;
            virtual void update() = 0;
            virtual void late_update() = 0;

        private:
            std::string mComponentName;
    };
}