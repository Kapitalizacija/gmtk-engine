#pragma once

#include <stdfloat>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <optional>
#include <vector>
#include <cxxabi.h>
#include <cstdlib>

#include <glad/glad.h>

#include "components/component.hpp"
#include "io/logging/logger.hpp"
#include "scene/ref/resource_ref.hpp"

namespace Sierra {
    class Scene;

    class Object {
        friend class Scene;
        friend class Renderer2D;
        friend class RenderBatch2D;

        public:
            Object();
            virtual ~Object() = default;

            Object(const Object&) = delete;
            Object(Object&&) = delete;

            template <class T, typename... Args>
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

            bool getEnabled() { return enabled; }
            void setEnabled(bool status) { enabled = status; }

            bool isRendered();
            
        protected:
            virtual void start();
            virtual void earlyUpdate(const StateInfo& stateInfo);
            virtual void update(const StateInfo& stateInfo);
            virtual void lateUpdate(const StateInfo& stateInfo);
            virtual void fixedUpdate(const StateInfo& stateInfo);
            virtual std::vector<float> getDrawData();
            virtual void frameCleanup();

            bool hasChanged();

            std::string mObjectName;
            std::unordered_map<size_t, std::shared_ptr<Component::Component>> mComponents;
            std::unordered_set<std::string> mTags;

            ResourceRef<Scene> mScene;
    
            bool enabled;
            bool rendered;
            bool changed;
    };

    #include "object.tpp"
}