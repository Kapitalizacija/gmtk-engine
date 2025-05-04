#pragma once

#include <glad/glad.h>

#include "../buffer/gl_buffer.hpp"

namespace GMTKEngine {
    struct GLAttribPointer {
        GLBuffer* buff;

        GLuint index;
        GLuint component_count;
        GLenum type;
        GLsizei stride;
        GLuint offset;
    };

    class GLVAO {

        public:
            GLVAO(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count);

            GLVAO(const GLVAO& other) = delete;

            GLVAO(GLVAO&& other);
            void operator=(GLVAO&& other);

            ~GLVAO();

            bool is_valid();
            GLuint get_vao();
        private:
            void create_vao();
            void create_attrib_pointers(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count);

            void move(GLVAO& other);

            GLuint vao;
    };
}