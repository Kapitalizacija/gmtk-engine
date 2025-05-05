#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "object/object.hpp"
#include "object/object2D.hpp"
#include "object/object3D.hpp"
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

            template <class T>
            void enable_rendering(T object) {
                if (objects.find(object) == objects.end()) {
                    ERROR("Tried to enable rendering on a nonexistant object");
                    return;
                }

                typedef std::remove_pointer_t<T> deref_T;

                if(std::is_base_of<Object2D, deref_T>::value) {
                    add_object2d(object);
                } else if (std::is_base_of<Object3D, deref_T>::value)
                {
                    throw new std::runtime_error("UNIMPLEMENTED");
                } else {
                    throw new std::runtime_error("UNIMPLEMENTED");
                }
            }

            template <class T>
            void disable_rendering(T object) {
                if (objects.find(object) == objects.end()) {
                    ERROR("Tried to disable rendering on a nonexistant object");
                    return;
                }

                typedef std::remove_pointer_t<T> deref_T;

                if(std::is_base_of<Object2D, deref_T>::value) {
                    remove_object2d(object);
                } else if (std::is_base_of<Object3D, deref_T>::value)
                {
                    throw new std::runtime_error("UNIMPLEMENTED");
                } else {
                    throw new std::runtime_error("UNIMPLEMENTED");
                }
            }
            
            void start();
            void update();
    
        private:
            void add_object2d(Object2D* object);
            void remove_object2d(Object2D* object);

            uint64_t combine_hashes(uint64_t hash1, uint64_t hash2);

            std::unordered_set<Object*> objects;

            /*
            hash is made by combining the shader program id and 
            the type hash so we can know what can be grouped together,
             will be reordered as needeed
            */ 

            //think of this as a map, with all the hashes that as a value, has
            //a vector of objects each with different sprites batched in groups of 32
            //(the max OpenGL allows the bound at once)
            std::unordered_map<size_t, std::vector<std::unordered_map<GLuint, std::unordered_set<Object2D*>>>> draw_batches_2d;

            Camera camera;
    };
}