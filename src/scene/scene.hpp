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
#include "renderer/dbg/debug_renderer.hpp"
#include "renderer/renderer.hpp"
#include "util/utilities.hpp"
#include "physics_manager/2d/physics_manager2d.hpp"
#include "dt/delta_time.hpp"
#include "input/input.hpp"

namespace Sierra {
 
    class Scene {
        public:
            Scene(ResourceRef<Window> window);

            Scene(const Scene&) = delete;
            Scene(const Scene&&) = delete;
            
            template<class T, typename... Args>
            ResourceRef<T> createObject(Args&... args);
            
            template<class T>
            void destroyObject(ResourceRef<T> object);
            
            ResourceRef<Renderer> getRenderer();
            ResourceRef<PhysicsManager2D> getPhysicsManager2D();
            
            void start();
            void update();
            void fixedUpdate();
            
            ResourceRef<Camera> getCamera();
            float getDeltaTime();

            void freeUnusedMemory();
    
        private:
            DeltaTime dt;
            InputHandler mInputHandler;

            StateInfo mStateInfo;

            std::unordered_set<std::shared_ptr<Object>> objects;

            std::shared_ptr<Renderer> renderer;
            std::shared_ptr<PhysicsManager2D> physicsManager2D;

            const uint64_t FIXED_UPDATE_INTERVAL = FIXED_TIMESTEP_MS;
            uint64_t sinceLastFixedUpdate = 0;
    };

    #include "scene.tpp"
}