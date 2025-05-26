#include "gl_buffer.hpp"

#define UNDEFINED_WARN WARN("Tried to create a buffer of UNDEFINED type")

namespace GMTKEngine {
    GLBuffer::GLBuffer(): buff(0) {
        type = Type::UNDEFINED; 
        size = 0;
    }

    GLBuffer::GLBuffer(Type buffer_type): buff(0), type(buffer_type) {
        if (buffer_type == Type::UNDEFINED) {
            UNDEFINED_WARN;
        }

        size = 0;

        create_buffer();
        matchType(buffer_type);
    }


    GLBuffer::GLBuffer(GLBuffer&& other) noexcept{
        move(other);
    }

    void GLBuffer::operator=(GLBuffer&& other) noexcept{
        move(other);
    }


    GLBuffer::~GLBuffer() {
        if ( !isValid() ) {
            return;
        }

        glDeleteBuffers(1, &buff);
    }

    void GLBuffer::move(GLBuffer& other) noexcept{
        buff = other.buff;
        type = other.type;
        glType = other.glType;
        
        other.buff = 0;
        other.glType = 0;
        other.type = Type::UNDEFINED;
    }

    void GLBuffer::create_buffer() {
        glCreateBuffers(1, &buff);
    }

    bool GLBuffer::isValid() {
        return buff != 0;
    }
    
    GLuint GLBuffer::getBuffer() {
        return buff;
    }

    size_t GLBuffer::getSize() {
        return size;
    }
    
    GLBuffer::Type GLBuffer::getType() {
        return type;
    }

    bool GLBuffer::assert_valid() {
        if ( !isValid() ) {
            ERROR("Tried to use invalid buffer");
            return false;
        }

        return true;
    }
    
    void GLBuffer::matchType(Type buffer_type) {
        switch (buffer_type) {
            case Type::VERTEX:
                glType = GL_ARRAY_BUFFER;
                break;
            case Type::INDEX:
                glType = GL_ELEMENT_ARRAY_BUFFER;
                break;
            case Type::STORAGE:
                glType = GL_SHADER_STORAGE_BUFFER;
                break;
            case Type::UNDEFINED:
                UNDEFINED_WARN;
                
                glDeleteBuffers(1, &buff);
                buff = 0;

                return;
        }
    }

    void GLBuffer::bindBase(GLuint index) {
        if (type != Type::STORAGE) {
            ERROR("Tried to bind buffer base on non storage buffer: " << buff);
            return;
        }

        glBindBufferBase(glType, index, buff);
    }

    void GLBuffer::partialUpdate(std::vector<GLBufferUpdateRegion> updateRegions) {

        glBindBuffer(glType, buff); 

        for (GLBufferUpdateRegion& region : updateRegions) {
            if (region.offset >= 221136){
                continue;
            } 
            glBufferSubData(glType, region.offset, region.size, region.data + region.offset);
        }
}
}