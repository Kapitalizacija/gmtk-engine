#pragma once

#include "../component.hpp"
#include <glad/glad.h>

#include "gl/texture/gl_texture.hpp"

namespace GMTKEngine {
    class Texture : public Component {
        friend class Scene;
        friend class Renderer2D;

        public:
            Texture();
            ~Texture();
        
            void setTexture(GLTexture& gl_texture);
            GLuint get_raw_handle();

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }

        private:
            GLuint mTextureID;
            std::string mComponentName;
    };
}