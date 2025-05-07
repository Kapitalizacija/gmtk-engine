#include "gl_vao.hpp"

namespace GMTKEngine {

    GLVAO::GLVAO(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count) {
        create_vao();
        create_attrib_pointers(attrib_pointers, attrib_pointer_count);
    }

    GLVAO::GLVAO(GLVAO&& other) {
        move(other);
    }

    void GLVAO::operator=(GLVAO&& other) {
        move(other);
    }

    GLVAO::~GLVAO() {
        if ( !is_valid() ) {
            return;
        }

        glDeleteVertexArrays(1, &vao);
    }

    bool GLVAO::is_valid() {
        return vao != 0;
    }

    GLuint GLVAO::get_vao() {
        return vao;
    }
    
    void GLVAO::create_vao() {
        glCreateVertexArrays(1, &vao);
    }
    
    void GLVAO::create_attrib_pointers(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count) {
        glBindVertexArray(vao);

        for (size_t i = 0; i < attrib_pointer_count; i++) {
            GLAttribPointer *attrib = &attrib_pointers[i];

            glBindBuffer(GL_ARRAY_BUFFER, attrib->buff->get_buffer());

            glEnableVertexAttribArray(attrib->index);
            glVertexAttribPointer(attrib->index, attrib->component_count, attrib->type, GL_FALSE, attrib->stride, (const void*)attrib->offset);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glBindVertexArray(0);
    } 

    void GLVAO::move(GLVAO& other) {
        vao = other.vao;

        other.vao = 0;
    }
    
}