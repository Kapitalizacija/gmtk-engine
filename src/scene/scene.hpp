#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "object/object.hpp"
#include "object/2d/object2D.hpp"
#include "object/3d/object3D.hpp"
#include "object/camera/camera.hpp"
#include "renderer/2d/renderer2d.hpp"

namespace GMTKEngine {
    class Scene {
        public:
            Scene();

            Scene(const Scene& other) = delete;
            Scene(Scene&& other) = delete;

            ~Scene();

            void freeUnusedMemory();

            //"Create" is a bit misleading, you have to create an instance of the class and then pass a pointer for it. Same goes for components in the Object 
            template<class T>
            void create_object(T* object) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, deref_T>::value);

                *object = new deref_T;
            
                objects.insert((Object*)*object);
            }
    
            template<class T>
            void destroy_object(T* object) {
                typedef std::remove_pointer_t<T> deref_T;

                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, deref_T>::value);

                delete *object;

                
                if (object->rendered) {
                    renderer2d.removeObject2d((Object2D*)*object);
                }
                objects.erase((Object*)*object);

                *object = nullptr;
            }

            template <class T>
            void addToRenderer(T object) {
                if (objects.find(object) == objects.end()) {
                    ERROR("Tried to enable rendering on a nonexistant object");
                    return;
                }

                typedef std::remove_pointer_t<T> deref_T;

                if(std::is_base_of<Object2D, deref_T>::value) {
                    renderer2d.addObject2d(object);
                } else if (std::is_base_of<Object3D, deref_T>::value)
                {
                    throw new std::runtime_error("UNIMPLEMENTED");
                } else {
                    throw new std::runtime_error("UNIMPLEMENTED");
                }
            }

            template <class T>
            void removeFromRenderer(T object) {
                if (objects.find(object) == objects.end()) {
                    ERROR("Tried to disable rendering on a nonexistant object");
                    return;
                }

                typedef std::remove_pointer_t<T> deref_T;

                if(std::is_base_of<Object2D, deref_T>::value) {
                    renderer2d.removeObject2d(object);
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

            std::unordered_set<Object*> objects;

            Camera camera;

            Renderer2D renderer2d;
    };
}