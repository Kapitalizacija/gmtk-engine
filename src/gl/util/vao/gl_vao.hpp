#pragma once

#include <glad/glad.h>

#include "../buffer/gl_buffer.hpp"

namespace Sierra {
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

            GLVAO(GLVAO&& other) noexcept;
            void operator=(GLVAO&& other) noexcept;

            ~GLVAO();

            void addEBO(GLBuffer& buff);

            bool isValid();
            GLuint getVAO();
        private:
            void createVAO();
            void createAttribPointers(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count);

            void move(GLVAO& other) noexcept;

            GLuint vao;
    };
}