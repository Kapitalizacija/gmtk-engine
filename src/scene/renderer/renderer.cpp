#include "renderer.hpp"

namespace GMTKEngine {
    Renderer::Renderer() {
        camera = std::make_shared<Camera>();
    }

    std::weak_ptr<Renderer2D> Renderer::getRenderer2D() {
        return renderer2d;
    }

    void Renderer::renderer(){
        renderer2d->render(*camera);
    }

    void Renderer::freeUnusedMemory() {
        renderer2d->freeUnusedMemory();
    }
}
