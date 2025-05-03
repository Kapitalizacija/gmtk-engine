#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>

#include "object/object.hpp"

namespace GMTKEngine {
    class Manager {
        public:
            Manager();
            ~Manager();

            //"Create" is a bit misleading, you have to create an instance of the class and then pass a pointer for it. Same goes for components in the Object 
            template<class T>
            void create_object(T* object) {
                static_assert(std::is_base_of<Object, std::remove_pointer_t<T>>::value);
            
                objects.insert((Object*)*object);
            }
    
            template<class T>
            void destroy_object(T* object) {
                static_assert(std::is_base_of<Object, std::remove_pointer_t<T>>::value);
            
                objects.erase((Object*)*object);
            }
            
            void start();
            void update();
    
        private:
            std::unordered_set<Object*> objects;
    };
}