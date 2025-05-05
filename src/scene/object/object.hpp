#pragma once

#include <stdfloat>
#include <iostream>
#include <unordered_set>

#include <glad/glad.h>

#include "components/component.hpp"

namespace GMTKEngine {
    class Object {
        friend class Scene;

        public:
            Object();
            ~Object();

            Object(const Object& other) = delete;
            Object(Object&& other) = delete;

            template<class T>
            void create_component(T* component) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Component, deref_T>::value);

                *component = new deref_T;
            
                mComponents.insert((Component*)*component);
            }

            std::string getName() { return mObjectName; }
            void setName(std::string name) { mObjectName = name; }
            
            void addTag(std::string tag) { mTags.insert(tag); }
            bool hasTag(std::string tag) { return mTags.count(tag); }
            void removeTag(std::string tag) { mTags.erase(tag); }
            
        protected:
            virtual void start();
            virtual void early_update();
            virtual void update();
            virtual void late_update();
            virtual void draw();

            std::string mObjectName;
            std::unordered_set<Component*> mComponents;
            std::unordered_set<std::string> mTags;

            GLuint program;
    
            bool enabled;
    };
}