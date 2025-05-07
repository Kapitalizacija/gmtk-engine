#pragma once

#include <cstdlib>
#include <glad/glad.h>

#include "io/logging/logger.hpp"

namespace GMTKEngine {



    class GLBuffer {
        public:
            enum GLBufferType {
                UNDEFINED,
                VERTEX,
                INDEX
            };

            enum GLBufferUsage {
                STREAM,
                OFTEN,
                RARELY
            };

            GLBuffer();
            GLBuffer(GLBufferType buffer_type);

            template<typename T>
            GLBuffer(T data, size_t size, GLBufferType buffer_type, GLBufferUsage buffer_usage);
            
            GLBuffer(const GLBuffer& other) = delete;

            GLBuffer(GLBuffer&& other);
            void operator=(GLBuffer&& other);

            ~GLBuffer();

            template<typename T>
            void upload_data(T data, size_t size, GLBufferUsage buffer_usage);

            bool is_valid();
            size_t get_size();
            GLuint get_buffer();
            GLenum get_type();
        private:
            bool assert_valid();
            void create_buffer();
            void match_type(GLBufferType buffer_type);

            void move(GLBuffer& other);

            GLuint type;
            GLuint buff;
            size_t size;
    };

    template<typename T>
    GLBuffer::GLBuffer(T data, size_t size, GLBufferType buffer_type, GLBufferUsage buffer_usage): size(0) {
        if (buffer_type == UNDEFINED) {
            WARN("Tried to create a buffer of undefined type");
        }
        
        this->size = size;
        create_buffer();
        match_type(buffer_type);
        upload_data(data, size, buffer_usage);
    }

    template<typename T>
    void GLBuffer::upload_data(T data, size_t size, GLBufferUsage buffer_usage) {
        static_assert(std::is_pointer<T>::value);

        if ( !assert_valid() ) {
            return;
        }

        this->size = size;

        GLuint usage;
        switch (buffer_usage) {
            case STREAM:
                usage = GL_STREAM_DRAW;
                break;
            case OFTEN:
                usage = GL_DYNAMIC_DRAW;
                break;
            case RARELY:
                usage = GL_STATIC_DRAW;
                break;
        }

        glBindBuffer(type, buff);
        glBufferData(type, size, data, usage);
        glBindBuffer(type, 0);
    }

}