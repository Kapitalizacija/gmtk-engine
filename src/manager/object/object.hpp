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

            template<class T>
            void create_component(T* comp) {
                static_assert(std::is_base_of<Component, std::remove_pointer_t<T>>::value);

                mComponents.insert((Component*)*comp);
            }
    
            template<class T>
            void destroy_component(T* comp) {
               
                static_assert(std::is_base_of<Component, std::remove_pointer_t<T>>::value);
            
                mComponents.erase((Component*)*comp);
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