#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "font/font.hpp"
#include "scene/object/custom_render_object/custom_render_object.hpp"
#include "scene/object/2d/object2d.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/vao/gl_vao.hpp"

namespace Sierra {
    class Text : public Object, public CustomRenderObject {
        public:
            Text(Font& font, std::string text);

            Text(Text&) = delete;
            Text(Text&&) = delete;

            ~Text();

        protected:
            void draw(Camera& camera) override;

        private:
            void createBuffers(std::vector<float> offsetData);

            static void createGlobalResources();

            GLVAO vao;
            GLBuffer offsetGLBuffer;

            Font& font;
            std::string text;

            static std::shared_ptr<GLShader> SHADER;
            static std::shared_ptr<GLBuffer> VERTEXGLBUFF;
            static std::shared_ptr<GLBuffer> INDEXGLBUFF;
            static GLuint posUniformLocation;
            static GLuint scaleUniformLocation;
            static GLuint rotationUniformLocation;
            static GLuint textureUniformLocation;
            static GLuint projectionUniformLocation;


            static const std::string VERTEX_PATH;
            static const std::string FRAGMENT_PATH;
            static uint32_t USE_COUNT;
    };
}