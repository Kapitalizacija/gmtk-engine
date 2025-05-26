#include "EngineAPI.h"
#include "scene/object/object.hpp"
#include <string>

#include "io/logging/logger.hpp"
#include "util/utilities.hpp"

namespace GMTKEngine {
    class GameObjectWrapper : public Scripting::GameObject {
        Object mObj;

        public:
            void SetName(const std::string &name) override {
                mObj.setName(name);
            }

            std::string GetName() override {
                return mObj.getName();
            }

            void SetActive(const bool enabled) override {
                mObj.setEnabled(enabled); 
            }

            bool GetActive() override {
                return mObj.getEnabled();
            }

            void AddTag(const std::string &name) override {
                mObj.addTag(name);
            }

            bool HasTag(const std::string &name) override {
                return mObj.hasTag(name);
            }

            void RemoveTag(const std::string &name) override {
                mObj.removeTag(name);
            }

            template <class T, typename... Args>
            T CreateComponent(Args&... args) {
                return mObj.createComponent<T>(args...);
            }

            template <class T>
            T GetComponent() {
                return mObj.getComponent<T>();
            }

            void Destroy() override {
                WARN("Not implemented");
                return;
            }
    };
}

API_EXPORT GMTKEngine::Scripting::GameObject *CreateGameObject() {
    return new GMTKEngine::GameObjectWrapper();
}