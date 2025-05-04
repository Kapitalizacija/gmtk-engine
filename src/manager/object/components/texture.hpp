#pragma once

#include "component.hpp"
#include <glad/glad.h>

namespace GMTKEngine {
    class Texture : public Component {
        public:
            Texture();
            ~Texture();

            virtual void start() { return; }
            virtual void early_update() { return; }
            virtual void update() { return; }
            virtual void late_update() { return; }
        private:
            GLuint64 mTextureID;
            std::string mComponentName;
    };
}