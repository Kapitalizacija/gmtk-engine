#pragma once

#include "scene/ref/resource_ref.hpp"
#include "scene/object/camera/camera.hpp"
#include <string>
#include <memory>

namespace Sierra::Scripting {
    struct Vector2 {
        float x;
        float y;
    };
    
    struct Vector3 {
        float x;
        float y;
        float z;
    };

    struct Vector4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct GameObject {
        virtual void SetName(const std::string &name) = 0;
        virtual std::string GetName() = 0;
        virtual void SetActive(const bool enabled) = 0;
        virtual bool GetActive() = 0;
        virtual void AddTag(const std::string &name) = 0;
        virtual bool HasTag(const std::string &name) = 0;
        virtual void RemoveTag(const std::string &name) = 0;

        template <class T, typename... Args>
        T CreateComponent(Args&... args);

        template <class T>
        T GetComponent();

        virtual void Destroy() = 0;
        virtual ~GameObject() = default;
    };

    struct Scene {
        virtual std::shared_ptr<Camera> GetCamera();
        
        template <typename... Args>
        GameObject CreateObject(Args&... args);

        void DestroyObject(GameObject &object);
    };
}

extern "C" Sierra::Scripting::GameObject *CreateGameObject();
extern "C" Sierra::Scripting::Scene *CreateScene();