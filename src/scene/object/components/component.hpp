#pragma once

#include <string>
#include "io/logging/logger.hpp"

namespace GMTKEngine {
    class Component {
        friend class Object;

        public:
            std::string getComponentName();

            virtual ~Component() = default;

        protected:
            virtual void start() = 0;
            virtual void early_update() = 0;
            virtual void update() = 0;
            virtual void late_update() = 0;
            virtual bool changedSinceLastUpdate() = 0; 
            
            std::string mComponentName;

        private:
    };
}