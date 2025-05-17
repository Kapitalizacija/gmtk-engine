#pragma once

#include <memory>
#include <stdexcept>

#include "scene/object/2d/object2d.hpp"
#include "scene/object/3d/object3d.hpp"
#include "scene/object/custom_render_object/custom_render_object.hpp"
#include "scene/object/components/ref/component_ref.hpp"
#include "2d/renderer2d.hpp"

namespace GMTKEngine {
    typedef std::unordered_set<std::weak_ptr<CustomRenderObject>, WeakPtrObjectHash<CustomRenderObject>, WeakPtrObjectEquals<CustomRenderObject>> CustomObjectTable;

    class Renderer {
        friend class Scene;

        public:
            Renderer();

            template<typename T>
            void addToRenderer(std::weak_ptr<T> object);

            void renderer();
            void freeUnusedMemory();

            std::weak_ptr<Renderer2D> getRenderer2D();

        protected:
            std::shared_ptr<Camera> camera;

        private:
            std::shared_ptr<Renderer2D> renderer2d;

            CustomObjectTable renderObjects;
    };

    #include "renderer.tpp"
}