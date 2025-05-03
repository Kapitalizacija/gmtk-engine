#include "component.hpp"

namespace GMTKEngine {
    class TestComponent : public Component {
        public:
            TestComponent();
            ~TestComponent();
            virtual void start();
            virtual void early_update();
            virtual void update();
            virtual void late_update();

        private:
            std::string mComponentName;
    };
}