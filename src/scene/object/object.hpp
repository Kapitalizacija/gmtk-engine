#pragma once

#include <stdfloat>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>

#include "components/component.hpp"

namespace GMTKEngine {
    class Object {
        friend class Scene;
        friend class Renderer2D;

        public:
            Object();
            virtual ~Object();

            Object(const Object& other) = delete;
            Object(Object&& other) = delete;

            template<class T>
            void createComponent(T* component) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Component, deref_T>::value);

                *component = new deref_T();

                mComponents[(*component)->getComponentName()] = *component;
                
            }

            std::string getName() { return mObjectName; }
            void setName(std::string name) { mObjectName = name; }

            Component* getComponent(std::string componentName);
            
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

            bool changedSinceLastUpdate();

            std::string mObjectName;
            std::unordered_map<std::string, Component*> mComponents;
            std::unordered_set<std::string> mTags;
    
            bool enabled;
            bool rendered;
            bool changed;
    };
}