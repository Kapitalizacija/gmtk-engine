#include "TestComponent.hpp"

namespace GMTKEngine {
    TestComponent::TestComponent() {
        mComponentName = "TestComponent";
        LOG("Component Awoke");
    }

    void TestComponent::start() {
        LOG("Component Started");
    }

    void TestComponent::update() {
        LOG("Component Updated");
    }

    void TestComponent::early_update() {
        LOG("Component Early Updated");
    }

    void TestComponent::late_update() {
        LOG("Component Late Updated");
    }
}