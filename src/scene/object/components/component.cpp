#include "component.hpp"

namespace GMTKEngine {
    namespace Component {
        Component::Component(std::weak_ptr<Object> ref) {
            mGameObject = ResourceRef<Object>(ref);
        }

        std::string Component::getComponentName() {
            return mComponentName;
        }

        std::vector<size_t> Component::getRequiredComponentHashes() const{
            return {};
        }

        void Component::setRequiredComponents(std::vector<ResourceRef<Component>> components) {
            assert(components.empty());
        }
    }
}
