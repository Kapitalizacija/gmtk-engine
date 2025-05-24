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

            GLBuffer(GLBuffer&& other) noexcept;
            void operator=(GLBuffer&& other) noexcept;

            ~GLBuffer();

            void bindBase(GLuint index);

            template<typename T>
            void uploadData(T data, size_t size, Usage buffer_usage);

            void partialUpdate(std::vector<GLBufferUpdateRegion> updateRegions);

            bool isValid();
            size_t getSize();
            GLuint getBuffer();
            Type getType();
        private:
            bool assert_valid();
            void create_buffer();
            void matchType(Type buffer_type);

            void move(GLBuffer& other) noexcept;

            Type type;
            GLenum glType;

            GLuint buff;
            size_t size;
    };

    #include "gl_buffer.tpp"

}