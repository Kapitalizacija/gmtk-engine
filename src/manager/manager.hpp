#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>

#include "object/object.hpp"

class Manager {
    public:
        Manager();

        template<class T>
        void create_object(T** object) {
            static_assert(std::is_base_of<Object, T>::value, "Class must derive from *Object*");
        
            *object = new T;
        
            objects.insert((Object*)*object);
        }

        template<class T>
        void destroy_object(T* object) {
            static_assert(std::is_base_of<Object, T>::value, "Class must derive from *Object*");
        
            objects.erase((Object*)object);
        }

        void update();

    private:
        std::unordered_set<Object*> objects;
};