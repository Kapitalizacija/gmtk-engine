#pragma once

#include <glad/glad.h>

#include "../object.hpp"
#include "../components/texture/texture.hpp"
#include "../components/transform/2d/transform2d.hpp"
#include "gl/util/shader/gl_shader.hpp"

namespace GMTKEngine {
    class Object2D : public Object {
        friend class Renderer2D;

        public:
            Object2D();

            void setShader(GLShader& shader);

        protected:
            std::vector<float> getDrawData() override;

            GLuint program;
    };
}