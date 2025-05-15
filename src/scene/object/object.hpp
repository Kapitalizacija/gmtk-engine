#pragma once

#include <stdfloat>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <optional>
#include <vector>

#include <glad/glad.h>

#include "components/component.hpp"
#include "io/logging/logger.hpp"

namespace GMTKEngine {
    class Object {
        friend class Scene;
        friend class Renderer2D;

        public:
            Object();
            virtual ~Object() = default;

            Object(const Object&) = delete;
            Object(Object&&) = delete;

            template<class T, typename... Args>
            std::optional<std::weak_ptr<T>> createComponent(Args... args) {
                static_assert(std::is_base_of<Component, T>::value);
                static_assert(!std::is_pointer<T>::value);
                
                if (mComponents.find(typeid(T).hash_code()) != mComponents.end()) {
                    WARN("Tried to add a preexisting component to object");
                    return std::nullopt; 
                }

                changed = true;
                
                std::shared_ptr<T> component = std::make_shared<T>(args...);

                mComponents[typeid(T).hash_code()] = component;
                
                return component;
            }

            std::string getName() { return mObjectName; }
            void setName(std::string name) { mObjectName = name; }

            template <class T>
            std::optional<std::weak_ptr<T>> getComponent() {
                auto it = mComponents.find(typeid(T).hash_code());

                if (it == mComponents.end()) {
                    return std::nullopt;
                }

                return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
            }

            template <typename T>
            std::optional<std::shared_ptr<T>> getComponentLock() {
                std::optional<std::weak_ptr<T>> weakPtr = getComponent<T>();

                if (weakPtr.has_value()) {
                    return weakPtr.value().lock();
                }

                return std::nullopt;
            }

            template <class T>
            bool changedComponent() {
                return mComponents[typeid(T).hash_code()]->hasChanged();
            }
            
            void addTag(std::string tag) { mTags.insert(tag); }
            bool hasTag(std::string tag) { return mTags.count(tag); }
            void removeTag(std::string tag) { mTags.erase(tag); }

            bool isRendered();
            
        protected:
            virtual void start();
            virtual void earlyUpdate();
            virtual void update();
            virtual void lateUpdate();
            virtual std::vector<float> getDrawData();
            virtual void frameCleanup();

            bool hasChanged();

            std::string mObjectName;
            std::unordered_map<size_t, std::shared_ptr<Component>> mComponents;
            std::unordered_set<std::string> mTags;
    
            bool enabled;
            bool rendered;
            bool changed;
    };
}