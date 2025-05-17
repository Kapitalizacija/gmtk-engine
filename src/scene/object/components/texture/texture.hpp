#pragma once

#include "../component.hpp"
#include <glad/glad.h>

#include "gl/texture/gl_texture.hpp"

namespace GMTKEngine {
    namespace Component {
        class Texture : public Component {
            friend class Scene;
            friend class Renderer2D;

            public:
                Texture();
            
                void setTexture(GLTexture& gl_texture);
                GLuint getRawHandle();

                virtual void start() { return; }
                virtual void earlyUpdate() { return; }
                virtual void update() { return; }
                virtual void lateUpdate() { return; }
                virtual void fixedUpdate() { return; }
                virtual void frameCleanup() override;
                virtual bool hasChanged() override;

            private:
                bool changed;

                GLuint mTextureID;
        };

    }
}