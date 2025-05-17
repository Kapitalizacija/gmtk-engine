#pragma once

#include <string>
#include "io/logging/logger.hpp"

namespace GMTKEngine {
    namespace Component {
        class Component {
            friend class Object;

            public:
                std::string getComponentName();

                virtual ~Component() = default;

                virtual void start() = 0;
                virtual void earlyUpdate() = 0;
                virtual void update() = 0;
                virtual void lateUpdate() = 0;
                virtual void fixedUpdate() = 0;
                virtual void frameCleanup() = 0;
                virtual bool hasChanged() = 0; 
            protected:

                std::string mComponentName;

            private:
        };
    }
}