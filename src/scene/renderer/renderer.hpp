#pragma once

#include <memory>
#include <stdexcept>

#include "scene/object/2d/object2d.hpp"
#include "scene/object/3d/object3d.hpp"
#include "scene/object/custom_render_object/custom_render_object.hpp"
#include "scene/ref/resource_ref.hpp"
#include "2d/renderer2d.hpp"

namespace Sierra {
    typedef std::unordered_set<ResourceRef<CustomRenderObject>> CustomObjectTable;

    class Renderer {
        friend class Scene;

        public:
            Renderer();

            void renderer();
            void freeUnusedMemory();

            ResourceRef<Renderer2D> getRenderer2d();

            template<typename T> 
            void addObject2d(ResourceRef<T> object);

            template<typename T> 
            void addCustomRenderObject(ResourceRef<T> object);

        protected:
            std::shared_ptr<Camera> mCamera;

        private:

            std::shared_ptr<Renderer2D> renderer2d;

            CustomObjectTable customRenderObjects;
    };

    #include "renderer.tpp"
}