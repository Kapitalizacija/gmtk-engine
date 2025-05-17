#pragma once

#include <memory>
#include <stdfloat>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>

#include "object/custom_render_object/custom_render_object.hpp"
#include "object/object.hpp"
#include "object/2d/object2d.hpp"
#include "object/3d/object3d.hpp"
#include "object/camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "util/utilities.hpp"

namespace GMTKEngine {
 
    class Scene {
        public:
            Scene();

            Scene(const Scene&) = delete;
            Scene(Scene&&) = delete;

            void freeUnusedMemory();

            template<class T, typename... Args>
            std::weak_ptr<T> createObject(Args&... args);

            template<class T>
            void destroyObject(std::weak_ptr<T> object);
       
            std::weak_ptr<Renderer> getRenderer();

            void start();
            void update();
            void fixedUpdate();

            std::weak_ptr<Camera> getCamera();
    
        private:

            std::unordered_set<std::shared_ptr<Object>> objects;

            std::shared_ptr<Renderer> renderer;

            const uint64_t FIXED_UPDATE_INTERVAL = 1000;
            uint64_t sinceLastFixedUpdate = 0;
    };

    #include "scene.tpp"
}