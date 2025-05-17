template<class T, typename... Args>
std::weak_ptr<T> Scene::createObject(Args&... args) {

    static_assert(!std::is_pointer_v<T>);
    static_assert(std::is_base_of_v<Object, T>);

    std::shared_ptr<T> obj = std::make_shared<T>(args...);
            
    objects.insert(obj);

    return obj;
}
    
template<class T>
void Scene::destroyObject(std::weak_ptr<T> object) {
    static_assert(!std::is_pointer_v<T>);
    static_assert(std::is_base_of_v<Object, T>);

    std::shared_ptr<T> shared_obj = object.lock();

    objects.erase(shared_obj);
}
