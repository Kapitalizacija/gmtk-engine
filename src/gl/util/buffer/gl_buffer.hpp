#pragma once

#include <cstdlib>
#include <vector>
#include <cstring>

#include <glad/glad.h>

#include "io/logging/logger.hpp"

namespace GMTKEngine {

    struct GLBufferUpdateRegion {
        uint8_t* data;
        size_t size;
        size_t offset;
    };

    class GLBuffer {
        public:
            enum class Type {
                UNDEFINED,
                VERTEX,
                STORAGE,
                INDEX
            };

            enum class Usage {
                STREAM,
                OFTEN,
                RARELY
            };

            GLBuffer();
            GLBuffer(Type buffer_type);

            template<typename T>
            GLBuffer(Type buffer_type, T data, size_t size, Usage buffer_usage);
            
            GLBuffer(const GLBuffer& other) = delete;

            GLBuffer(GLBuffer&& other);
            void operator=(GLBuffer&& other);

            ~GLBuffer();

            void bindBase(GLuint index);

            template<typename T>
            void uploadData(T data, size_t size, Usage buffer_usage);

            template<typename T>
            void partialUpdate(std::vector<GLBufferUpdateRegion> updateRegions);

            bool isValid();
            size_t getSize();
            GLuint getBuffer();
            Type getType();
        private:
            bool assert_valid();
            void create_buffer();
            void matchType(Type buffer_type);

            void move(GLBuffer& other);

            Type type;
            GLenum glType;

            GLuint buff;
            size_t size;
    };

    template<typename T>
    GLBuffer::GLBuffer(Type buffer_type, T data, size_t size, Usage buffer_usage): size(0), type(buffer_type) {
        if (buffer_type == Type::UNDEFINED) {
            WARN("Tried to create a buffer of undefined type");
        }
        
        this->size = size;
        create_buffer();
        matchType(buffer_type);
        uploadData(data, size, buffer_usage);
    }

    template<typename T>
    void GLBuffer::uploadData(T data, size_t size, Usage buffer_usage) {
        static_assert(std::is_pointer<T>::value);

        if ( !assert_valid() ) {
            return;
        }

        this->size = size;

        GLuint usage;
        switch (buffer_usage) {
            case Usage::STREAM:
                usage = GL_STREAM_DRAW;
                break;
            case Usage::OFTEN:
                usage = GL_DYNAMIC_DRAW;
                break;
            case Usage::RARELY:
                usage = GL_STATIC_DRAW;
                break;
        }

        glBindBuffer(glType, buff);
        glBufferData(glType, size, data, usage);
        glBindBuffer(glType, 0);
    }
    
    template<typename T>
    void GLBuffer::partialUpdate(std::vector<GLBufferUpdateRegion> updateRegions) {

        glBindBuffer(glType, buff); 
        void* ptr = glMapBuffer(glType, GL_WRITE_ONLY);

        if (ptr == nullptr) {
             ERROR("Failed to map buffer: " << this->buff);
        }

        for (GLBufferUpdateRegion& region : updateRegions) {
            memcpy(ptr + region.offset, region.data, region.size);
        }

        glUnmapBuffer(glType);
    }

}