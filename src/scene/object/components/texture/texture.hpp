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
                virtual void earlyUpdate(float dt) { return; }
                virtual void update(float dt) { return; }
                virtual void lateUpdate(float dt) { return; }
                virtual void fixedUpdate() { return; }
                virtual void frameCleanup() override;
                virtual bool hasChanged() override;

            private:
                bool changed;

                GLuint mTextureID;
        };

    }
}