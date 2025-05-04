#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>
#include <unordered_map>
#include <list>

#include "object/object.hpp"
#include "object/camera/camera.hpp"

namespace GMTKEngine {
    class Scene {
        public:
            Scene();
            ~Scene();

            //"Create" is a bit misleading, you have to create an instance of the class and then pass a pointer for it. Same goes for components in the Object 
            template<class T>
            void create_object(T* object) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, deref_T>::value);

                *object = new deref_T;
//                draw_batches_2d[typeid(deref_T).hash_code()].push_back(*object);
            
                objects.insert((Object*)*object);
            }
    
            template<class T>
            void destroy_object(T* object) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, deref_T>::value);

                delete *object;
 //               draw_batches_2d[typeid(deref_T).hash_code()].erase(*object);
            
                objects.erase((Object*)*object);

                *object = nullptr;
            }
            
            void start();
            void update();
    
        private:
            std::unordered_set<Object*> objects;
            std::unordered_map<size_t, std::vector<std::vector<std::vector<Object*>>>> draw_batches_2d;

            Camera camera;
    };
}