#pragma once

#include <glad/glad.h>

#include "../object.hpp"
#include "../components/texture/texture.hpp"
#include "../components/transform/transform.hpp"
#include "gl/util/shader/gl_shader.hpp"

namespace GMTKEngine {
    class Object2D : public Object, public Texture, public Transform {
        friend class Renderer2D;

        public:
            void setShader(GLShader& shader);

        protected:
            std::vector<float> getDrawData() override;

            GLuint program;
    };
}