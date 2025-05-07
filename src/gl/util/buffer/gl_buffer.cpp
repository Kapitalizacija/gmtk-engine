#include "gl_buffer.hpp"

#define UNDEFINED_WARN WARN("Tried to create a buffer of UNDEFINED type")

namespace GMTKEngine {
    GLBuffer::GLBuffer() {
        type = UNDEFINED; 
    }

    GLBuffer::GLBuffer(GLBufferType buffer_type): buff(0) {
        if (buffer_type == UNDEFINED) {
            UNDEFINED_WARN;
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
        if ( !is_valid() ) {
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

    bool GLBuffer::is_valid() {
        return buff != 0;
    }
    
    GLuint GLBuffer::get_buffer() {
        return buff;
    }

    size_t GLBuffer::get_size() {
        return size;
    }
    
    GLenum GLBuffer::get_type() {
        return type;
    }

    bool GLBuffer::assert_valid() {
        if ( !is_valid() ) {
            ERROR("Tried to use invalid buffer");
            return false;
        }

        return true;
    }
    
    void GLBuffer::match_type(GLBufferType buffer_type) {
        switch (buffer_type) {
            case VERTEX:
                type = GL_ARRAY_BUFFER;
                break;
            case INDEX:
                type = GL_ELEMENT_ARRAY_BUFFER;
                break;
            case UNDEFINED:
                UNDEFINED_WARN;
                
                glDeleteBuffers(1, &buff);
                buff = 0;

                return;
        }
    }
}