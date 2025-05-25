#include "EngineAPI.h"
#include "scene/scene.hpp"
#include <string>

#include "io/logging/logger.hpp"
#include "util/utilities.hpp"

namespace GMTKEngine {
    class SceneWrapper : public Scripting::Scene {
        GMTKEngine::Scene mScene;

        public:
            std::shared_ptr<Camera> GetCamera() override {
                return mScene.getCamera().getLock();
            }

            template <typename... Args>
            GMTKEngine::Scripting::GameObject CreateObject(Args&... args) {
                return mScene.createObject<Object>(args);
            }

            void DestroyObject(GMTKEngine::Scripting::GameObject &object) {
                //mScene.destroyObject(ResourceRef<Object>(std::weak_ptr(object)));
                WARN("Not implemented");
            }
    };
}

API_EXPORT GMTKEngine::Scripting::Scene *CreateScene() {
    return new GMTKEngine::SceneWrapper();
}