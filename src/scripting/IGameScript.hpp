#pragma once

namespace GMTKEngine::Scripting {
    struct IGameScript {
        virtual void OnStart() = 0;
        virtual void OnEarlyUpdate(float deltaTime) = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnLateUpdate(float deltaTime) = 0;
        virtual void OnFixedUpdate() = 0;

        virtual ~IGameScript() = default;
    };

    IGameScript *CreateScriptInstance();
}