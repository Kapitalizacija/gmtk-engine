#pragma once

#include <glad/glad.h>

#include "../buffer/gl_buffer.hpp"

namespace GMTKEngine {
    struct GLAttribPointer {
        GLBuffer* buff;

        GLuint index;
        GLuint componentCount;
        GLenum type;
        GLsizei stride;
        GLuint offset;
        bool isInstanced;
    };

    class GLVAO {

        public:
            GLVAO();
            GLVAO(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count);

            GLVAO(const GLVAO& other) = delete;

            GLVAO(GLVAO&& other);
            void operator=(GLVAO&& other);

            ~GLVAO();

            void addEBO(GLBuffer& buff);

            bool isValid();
            GLuint getVAO();
        private:
            void createVAO();
            void createAttribPointers(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count);

            void move(GLVAO& other);

            GLuint vao;
    };
}