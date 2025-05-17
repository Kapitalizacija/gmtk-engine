#include "renderer.hpp"

namespace GMTKEngine {
    Renderer::Renderer() {
        camera = std::make_shared<Camera>();
        renderer2d = std::make_shared<Renderer2D>();
    }

    std::weak_ptr<Renderer2D> Renderer::getRenderer2d() {
        return renderer2d;
    }

    void Renderer::renderer(){
        renderer2d->render(*camera);

        for (auto& object : customRenderObjects) {
            object.lock()->draw(*camera);
        }
    }

    void Renderer::freeUnusedMemory() {
        renderer2d->freeUnusedMemory();
    }
}
