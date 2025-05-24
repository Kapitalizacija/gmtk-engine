#pragma once

#include <string>
#include <vector>

#include "io/logging/logger.hpp"
#include "scene/ref/resource_ref.hpp"

namespace GMTKEngine {
    class Object;

    namespace Component {
        class Component {
            friend class Object;

            public:
                std::string getComponentName();

                Component() = default;
                Component(std::weak_ptr<Object> ref);
                virtual ~Component() = default;

                virtual void start() = 0;
                virtual void earlyUpdate() = 0;
                virtual void update() = 0;
                virtual void lateUpdate() = 0;
                virtual void fixedUpdate() = 0;
                virtual void frameCleanup() = 0;
                virtual bool hasChanged() = 0;
            protected:
                ResourceRef<Object> mGameObject;

                virtual std::vector<size_t> getRequiredComponentHashes() const;
                virtual void setRequiredComponents(std::vector<ResourceRef> components);

                std::string mComponentName;

            private:
        };
    }
}