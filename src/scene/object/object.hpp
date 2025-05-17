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
#include "scene/ref/resource_ref.hpp"

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
            ResourceRef<T> createComponent(Args&... args);

            template <class T>
            ResourceRef<T> getComponent();

            template <class T>
            bool changedComponent();

            std::string getName() { return mObjectName; }
            void setName(std::string name) { mObjectName = name; }
            
            void addTag(std::string tag) { mTags.insert(tag); }
            bool hasTag(std::string tag) { return mTags.count(tag); }
            void removeTag(std::string tag) { mTags.erase(tag); }

            bool isRendered();
            
        protected:
            virtual void start();
            virtual void earlyUpdate();
            virtual void update();
            virtual void lateUpdate();
            virtual void fixedUpdate();
            virtual std::vector<float> getDrawData();
            virtual void frameCleanup();

            bool hasChanged();

            std::string mObjectName;
            std::unordered_map<size_t, std::shared_ptr<Component::Component>> mComponents;
            std::unordered_set<std::string> mTags;
    
            bool enabled;
            bool rendered;
            bool changed;
    };

    #include "object.tpp"
}