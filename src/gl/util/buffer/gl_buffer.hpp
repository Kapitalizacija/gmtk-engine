#pragma once

#include <cstdlib>
#include <glad/glad.h>

#include "io/logging/logger.hpp"

namespace GMTKEngine {



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

            template<typename T>
            void upload_data(T data, size_t size, Usage buffer_usage);

            bool is_valid();
            size_t get_size();
            GLuint get_buffer();
            Type get_type();
        private:
            bool assert_valid();
            void create_buffer();
            void match_type(Type buffer_type);

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
        match_type(buffer_type);
        upload_data(data, size, buffer_usage);
    }

    template<typename T>
    void GLBuffer::upload_data(T data, size_t size, Usage buffer_usage) {
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

}