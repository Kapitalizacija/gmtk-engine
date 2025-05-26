#pragma once

#include <memory>
#include <stdexcept>

#include "io/logging/logger.hpp"

namespace Sierra {

    template<typename T>
    class ResourceRef {
        public:
            ResourceRef() {
                hash = 0;
            }

            ResourceRef(std::weak_ptr<T> ptr): weakPtr(ptr) {
                hash = std::hash<std::shared_ptr<T>>{}(weakPtr.lock());
            }

            ResourceRef(std::shared_ptr<T> ptr): weakPtr(ptr) {
                hash = std::hash<std::shared_ptr<T>>{}(weakPtr.lock());
            }

            std::shared_ptr<T> operator->() const {
                if (is_empty()) {
                    throw std::runtime_error("Tried to access an empty component");
                }

                return weakPtr.lock();
            }

            bool is_empty() const {
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

            bool operator==(const ResourceRef &other) const noexcept {
                return !weakPtr.owner_before(other.weakPtr) && !other.weakPtr.owner_before(weakPtr);
            }

            size_t getHash() const {
                if (hash == 0) {
                    WARN("Tried to hash an empty ResourceRef container");
                }

                return hash;
            }

            template<typename G>
            operator ResourceRef<G>() {
                return ResourceRef<G>(
                    std::static_pointer_cast<G>(weakPtr.lock()) // reinterpret_cast?? NO
                );
            }

        private:
            std::weak_ptr<T> weakPtr;
            size_t hash;
    };
}

namespace std {
    template<typename T>
    struct hash<Sierra::ResourceRef<T>> {
        auto operator()(const Sierra::ResourceRef<T>& ref) const -> size_t {
            return ref.getHash();
        }
    };
}