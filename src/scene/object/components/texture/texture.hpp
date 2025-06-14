#pragma once

#include "../component.hpp"
#include <glad/glad.h>

#include "gl/texture/gl_texture.hpp"

namespace Sierra {
    namespace Component {
        class Texture : public Component {
            friend class Scene;
            friend class Renderer2D;

            public:
                Texture();
                Texture(GLTexture& gl_texture);
            
                void setTexture(GLTexture& gl_texture);
                GLuint getRawHandle();

                virtual void start() { return; }
                virtual void earlyUpdate(const StateInfo& stateInfo) { return; }
                virtual void update(const StateInfo& stateInfo) { return; }
                virtual void lateUpdate(const StateInfo& stateInfo) { return; }
                virtual void fixedUpdate(const StateInfo& stateInfo) { return; }
                virtual void frameCleanup() override;
                virtual bool hasChanged() override;

            private:
                bool changed;

                GLuint mTextureID;
        };

    }
}