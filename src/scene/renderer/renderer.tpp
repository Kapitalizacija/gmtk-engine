
template<typename T>
void Renderer::addToRenderer(std::weak_ptr<T> object) {
    if (std::is_base_of_v<CustomRenderObject, T>) {
        renderObjects.insert(object);
    } else if (std::is_base_of_v<Object2D, T>) {
        renderer2d->addObject(object);
    } else {
        throw std::runtime_error("UNIMPLEMENTED");
    }
}