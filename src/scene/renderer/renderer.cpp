#include "renderer.hpp"

namespace Sierra {
    Renderer::Renderer() {
        mCamera = std::make_shared<Camera>();
        renderer2d = std::make_shared<Renderer2D>();
    }

    ResourceRef<Renderer2D> Renderer::getRenderer2d() {
        return renderer2d;
    }

    void Renderer::renderer(){
        renderer2d->render(*mCamera);

        for (auto& object : customRenderObjects) {
            object->draw(*mCamera);
        }
    }

    void Renderer::freeUnusedMemory() {
        renderer2d->freeUnusedMemory();
    }
}
