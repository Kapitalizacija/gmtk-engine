#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>

#include "object/object.hpp"
#include "object/2d/object2d.hpp"
#include "object/3d/object3d.hpp"
#include "object/camera/camera.hpp"
#include "renderer/2d/renderer2d.hpp"

namespace GMTKEngine {
 

    class Scene {
        public:
            Scene();

            Scene(const Scene&) = delete;
            Scene(Scene&&) = delete;

            void freeUnusedMemory();

            //"Create" is a bit misleading, you have to create an instance of the class and then pass a pointer for it. Same goes for components in the Object 
            template<class T>
            std::weak_ptr<T> createObject() {

                static_assert(!std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, T>::value);

                std::shared_ptr<T> obj = std::make_shared<T>();
            
                objects.insert(obj);

                return obj;
            }
    
            template<class T>
            void destroy_object(std::weak_ptr<T> object) {
                static_assert(std::is_pointer<T>::value);
                static_assert(std::is_base_of<Object, T>::value);

                std::shared_ptr<T> shared_obj = object.lock();

                if (object->rendered) {
                    renderer2d.removeObject2d(object);
                }
                objects.erase(shared_obj);
            }

            template <class T>
            void addToRenderer(std::weak_ptr<T> object) {
                if (objects.find(object.lock()) == objects.end()) {
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
            void removeFromRenderer(std::weak_ptr<T> object) {
                if (objects.find(object.lock()) == objects.end()) {
                    ERROR("Tried to disable rendering on a nonexistant object");
                    return;
                }


                if(std::is_base_of<Object2D, T>::value) {
                    renderer2d.removeObject2d(object);
                } else if (std::is_base_of<Object3D, T>::value)
                {
                    throw new std::runtime_error("UNIMPLEMENTED");
                } else {
                    throw new std::runtime_error("UNIMPLEMENTED");
                }
            }
            
            void start();
            void update();

            Camera* getCamera();
    
        private:

            std::unordered_set<std::shared_ptr<Object>> objects;

            Camera camera;

            Renderer2D renderer2d;
    };
}