#pragma once

#include <memory>
#include <stdexcept>

namespace GMTKEngine {

    template<typename T>
    class ResourceRef {
        public:
            ResourceRef() {}
            ResourceRef(std::weak_ptr<T> ptr): weakPtr(ptr) {}
            ResourceRef(std::shared_ptr<T> ptr): weakPtr(ptr) {}

            std::shared_ptr<T> operator->() {
                if (is_empty()) {
                    throw std::runtime_error("Tried to access an empty component");
                }

                return weakPtr.lock();
            }

            bool is_empty() {
                return weakPtr.use_count() == 0;
            }

            std::shared_ptr<T> getLock() {
                if (is_empty()) {
                    throw std::runtime_error("Tried to access an empty component");
                }

                return weakPtr.lock();
            }

            std::weak_ptr<T> getPtr() {
                if (is_empty()) {
                    throw std::runtime_error("Tried to access an empty component");
                }

                return weakPtr;
            }

        private:
            std::weak_ptr<T> weakPtr;
    };
}