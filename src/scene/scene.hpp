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
            ResourceRef<T> createObject(Args&... args);

            template<class T>
            void destroyObject(ResourceRef<T> object);
       
            ResourceRef<Renderer> getRenderer();

            void start();
            void update();
            void fixedUpdate();

            ResourceRef<Camera> getCamera();
    
        private:

            std::unordered_set<std::shared_ptr<Object>> objects;

            std::shared_ptr<Renderer> renderer;

            const uint64_t FIXED_UPDATE_INTERVAL = FIXED_TIMESTEP_MS;
            uint64_t sinceLastFixedUpdate = 0;
    };

    #include "scene.tpp"
}