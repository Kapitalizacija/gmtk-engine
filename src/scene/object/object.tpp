template<class T, typename... Args>

ResourceRef<T> Object::createComponent(Args&... args) {
    static_assert(std::is_base_of_v<Component::Component, T>);
    static_assert(!std::is_pointer_v<T>);
    
    if (mComponents.find(typeid(T).hash_code()) != mComponents.end()) {
        WARN("Tried to add a preexisting component to object");
        return ResourceRef<T>();
    }

    changed = true;
    
    std::shared_ptr<T> component = std::make_shared<T>(args...);

    std::vector<size_t> dependencyHashes = component->getRequiredComponentHashes();
    std::vector<ResourceRef<Component::Component>> dependencies;
    dependencies.reserve(dependencyHashes.size());

    for (size_t dep : dependencyHashes) {
        auto it = mComponents.find(dep);

        if (it == mComponents.end()) {
            char* demangled_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr); // standard library retardation

            ERROR("Failed to satisfy all dependencies for component " << demangled_name);

            free(demangled_name);
            return {};
        }

        dependencies.push_back(ResourceRef<Component::Component>(it->second));

    }
    component->setRequiredComponents(dependencies);

    mComponents[typeid(T).hash_code()] = component;
    
    return ResourceRef<T>(component);
}


template <class T>
ResourceRef<T> Object::getComponent() {
    auto it = mComponents.find(typeid(T).hash_code());

    if (it == mComponents.end()) {
        return ResourceRef<T>();
    }

    return ResourceRef<T>(std::static_pointer_cast<T>(it->second));
}

template <class T>
bool Object::changedComponent() {
    return mComponents[typeid(T).hash_code()]->hasChanged();
}