#pragma once

#include <stdfloat>
#include <iostream>
#include <unordered_set>
#include "components/component.hpp"

namespace GMTKEngine {
    template <typename T> class remove_all_pointers{
        public:
            typedef T type;
    };

    class Object {
        friend class Manager;

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

    
        protected:
            virtual void start();
            virtual void early_update();
            virtual void update();
            virtual void late_update();
            virtual void draw();

            std::unordered_set<Component*> mComponents;
    
            bool enabled;
    };
}