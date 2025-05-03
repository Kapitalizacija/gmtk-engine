#include "gl_buffer.hpp"

namespace GMTKEngine {
    GLBuffer::GLBuffer(GLBufferType buffer_type): buff(0) {
        if (buffer_type == UNDEFINED) {
            WARN("Tried to create a buffer of undefined type");
        }

        create_buffer();
        match_type(buffer_type);
    }


    GLBuffer::GLBuffer(GLBuffer&& other) {
        move(other);
    }

    void GLBuffer::operator=(GLBuffer&& other) {
        move(other);
    }


    GLBuffer::~GLBuffer() {
        if ( buff == 0 ) {
            LOG("Buffer destructed before it was ever used");
            return;
        }

        glDeleteBuffers(1, &buff);
    }

    void GLBuffer::move(GLBuffer& other) {
        buff = other.buff;
        type = other.type;
        
        other.buff = 0;
        other.type = UNDEFINED;
    }

    void GLBuffer::create_buffer() {
        glCreateBuffers(1, &buff);
    }


    bool GLBuffer::assert_valid() {
        if ( buff == 0 ) {
            ERROR("Tried to use invalid buffer");
            return true;
        }

        return false;
    }
    
    void GLBuffer::match_type(GLBufferType buffer_type) {
        switch (buffer_type) {
            case VERTEX:
                type = GL_VERTEX_ARRAY;
                break;
            case INDEX:
                type = GL_INDEX_ARRAY;
                break;
        }
    }
}