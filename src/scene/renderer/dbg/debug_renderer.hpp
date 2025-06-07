#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/object/camera/camera.hpp"

#include "gl/util/shader/gl_shader.hpp"
#include "gl/util/gl_util.hpp"
#include "gl/util/vao/gl_vao.hpp"

namespace Sierra {
    namespace Debug {
        class Renderer{
            public:
                static void init(ResourceRef<Camera> camera);
                static void cleanup();

                static void drawRect(glm::vec3 pos, glm::vec2 size, glm::vec4 color, bool filled = false, bool useCamera = true);
            private:

                static std::shared_ptr<GLShader> SHADER;
                static std::shared_ptr<GLVAO> VAO;

                static GLint POS_UNIFORM_LOC;
                static GLint SIZE_UNIFORM_LOC;
                static GLint CAM_UNFIROM_LOC;
                static GLint COLOR_UNIFORM_LOC;

                static ResourceRef<Camera> CAMERA;
        };
    }
}