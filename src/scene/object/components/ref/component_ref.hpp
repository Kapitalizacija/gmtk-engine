#pragma once

#include <memory>
#include <stdexcept>

namespace GMTKEngine {

    template<typename T>
    class ComponentRef {
        public:
            ComponentRef() {}
            ComponentRef(std::weak_ptr<T> ptr): weakPtr(ptr) {}
            ComponentRef(std::shared_ptr<T> ptr): weakPtr(ptr) {}

            std::shared_ptr<T> operator->() {
                if (weakPtr.use_count() == 0) {
                    throw std::runtime_error("Tried to access an empty component");
                }

                return weakPtr.lock();
            }

            bool is_empty() {
                return weakPtr.use_count() == 0;
            }

        private:
            std::weak_ptr<T> weakPtr;
    };
}