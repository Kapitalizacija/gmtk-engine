
template<typename T> 
void Renderer::addObject2d(ResourceRef<T> object) {
    renderer2d->addObject(object);
}

template<typename T> 
void Renderer::addCustomRenderObject(ResourceRef<T> object) {
    customRenderObjects.insert((ResourceRef<CustomRenderObject>)object);
}

