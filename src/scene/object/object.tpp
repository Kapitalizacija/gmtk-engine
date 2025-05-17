template<class T, typename... Args>

ComponentRef<T> Object::createComponent(Args&... args) {
    static_assert(std::is_base_of_v<Component, T>);
    static_assert(!std::is_pointer_v<T>);
    
    if (mComponents.find(typeid(T).hash_code()) != mComponents.end()) {
        WARN("Tried to add a preexisting component to object");
        return ComponentRef<T>();
    }

    changed = true;
    
    std::shared_ptr<T> component = std::make_shared<T>(args...);

    mComponents[typeid(T).hash_code()] = component;
    
    return ComponentRef<T>(component);
}


template <class T>
ComponentRef<T> Object::getComponent() {
    auto it = mComponents.find(typeid(T).hash_code());

    if (it == mComponents.end()) {
        return ComponentRef<T>();
    }

    return ComponentRef<T>(std::static_pointer_cast<T>(it->second));
}

template <class T>
bool Object::changedComponent() {
    return mComponents[typeid(T).hash_code()]->hasChanged();
}