template<class T, typename... Args>
ResourceRef<T> Scene::createObject(Args&... args) {

    static_assert(!std::is_pointer_v<T>);
    static_assert(std::is_base_of_v<Object, T>);

    std::shared_ptr<T> obj = std::make_shared<T>(args...);
    obj->mScene = this;
            
    objects.insert(obj);

    return ResourceRef<T>(obj);
}
    
template<class T>
void Scene::destroyObject(ResourceRef<T> object) {
    static_assert(!std::is_pointer_v<T>);
    static_assert(std::is_base_of_v<Object, T>);

    objects.erase(object.getLock());
}
