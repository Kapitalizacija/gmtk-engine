#include "gl_vao.hpp"

namespace GMTKEngine {

    GLVAO::GLVAO(): vao(0){

    }
    GLVAO::GLVAO(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count) {
        createVAO();
        createAttribPointers(attrib_pointers, attrib_pointer_count);
    }

    GLVAO::GLVAO(GLVAO&& other) {
        move(other);
    }

    void GLVAO::operator=(GLVAO&& other) {
        move(other);
    }

    GLVAO::~GLVAO() {
        if ( !isValid() ) {
            return;
        }

        glDeleteVertexArrays(1, &vao);
    }

    bool GLVAO::isValid() {
        return vao != 0;
    }

    GLuint GLVAO::getVAO() {
        return vao;
    }
    
    void GLVAO::createVAO() {
        glCreateVertexArrays(1, &vao);
    }
    
    void GLVAO::createAttribPointers(GLAttribPointer* attrib_pointers, size_t attrib_pointer_count) {
        glBindVertexArray(vao);

        for (size_t i = 0; i < attrib_pointer_count; i++) {
            GLAttribPointer *attrib = &attrib_pointers[i];

            if (attrib->buff->get_type() != GLBuffer::Type::VERTEX) {
                ERROR("Tried to set a vertex attrib with a non-vertex buffer");
                continue;
            } 
            if (!attrib->buff->is_valid()) {
                ERROR("Tried to set a vertex attrib with an invalid vertex buffer");
                continue;
            } 

            glBindBuffer(GL_ARRAY_BUFFER, attrib->buff->get_buffer());

            glEnableVertexAttribArray(attrib->index);
            glVertexAttribPointer(attrib->index, attrib->component_count, attrib->type, GL_FALSE, attrib->stride, (const void*)attrib->offset);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glBindVertexArray(0);
    } 

    void GLVAO::addEBO(GLBuffer& buff) {
        if (buff.get_type() != GLBuffer::Type::INDEX) {
            ERROR("Tried to set the index buffer in VAO with non index buffer");
            return;
        }
        if (!buff.is_valid()) {
            return;
            ERROR("Tried to bind an invalid EBO to a VAO");
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff.get_buffer());

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLVAO::move(GLVAO& other) {
        vao = other.vao;

        other.vao = 0;
    }
    
}