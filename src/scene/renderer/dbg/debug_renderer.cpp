#include "debug_renderer.hpp"

namespace Sierra {
    namespace Debug {

        std::shared_ptr<GLShader> Renderer::SHADER = std::shared_ptr<GLShader>(nullptr);
        std::shared_ptr<GLVAO> Renderer::VAO = std::shared_ptr<GLVAO>(nullptr);;

        GLint Renderer::POS_UNIFORM_LOC = 0;
        GLint Renderer::SIZE_UNIFORM_LOC = 0;
        GLint Renderer::CAM_UNFIROM_LOC = 0;
        GLint Renderer::COLOR_UNIFORM_LOC = 0;

        ResourceRef<Camera> Renderer::CAMERA = ResourceRef<Camera>();

        void Renderer::init(ResourceRef<Camera> camera) {
            SHADER = std::make_shared<GLShader>("debug_rect", "test_shaders/debug_rect.vert", "test_shaders/debug_rect.frag");

            POS_UNIFORM_LOC = glGetUniformLocation(SHADER->getProgram(), "pos");
            SIZE_UNIFORM_LOC = glGetUniformLocation(SHADER->getProgram(), "size");
            CAM_UNFIROM_LOC = glGetUniformLocation(SHADER->getProgram(), "cam");
            COLOR_UNIFORM_LOC = glGetUniformLocation(SHADER->getProgram(), "color");

            GLAttribPointer ptr{0};
            ptr.buff = &GLUtils::getSquareVertexBuffer();
            ptr.componentCount = 2;
            ptr.type = GL_FLOAT;
            ptr.stride = 8;
            ptr.offset = 0;
            ptr.isInstanced = false;

            VAO = std::make_shared<GLVAO>(&ptr, 1);
            VAO->addEBO(GLUtils::getSquareIndexBuffer());

            CAMERA = camera;
        }

        void Renderer::drawRect(glm::vec3 pos, glm::vec2 size, glm::vec4 color, bool filled, bool useCamera) {
            if (SHADER.get() == nullptr) {
                ERROR("Tried to draw using the debug renderer whilst debugging isn't enabled");
                return;
            }

            if (filled) 
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glm::mat4 identityMat = glm::mat4(1.0f);

            glBindVertexArray(VAO->getVAO());
            glUseProgram(SHADER->getProgram());

            glUniform2fv(POS_UNIFORM_LOC, 1, glm::value_ptr(pos));
            glUniform2fv(SIZE_UNIFORM_LOC, 1, glm::value_ptr(size));
            glUniform4fv(COLOR_UNIFORM_LOC, 1, glm::value_ptr(color));

            if (useCamera)
                CAMERA->__applyProjection(CAM_UNFIROM_LOC, Camera::ProjectionType::ORTHOGRAPHIC); 
            else
                glUniform4fv(CAM_UNFIROM_LOC, 1, glm::value_ptr(identityMat));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        void Renderer::cleanup() {
            SHADER.reset();
            VAO.reset();
        }
    }
}