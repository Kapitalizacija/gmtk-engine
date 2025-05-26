#pragma once

#include <glad/glad.h>
#include <vector>
#include <optional>
#include <cstddef>
#include <algorithm>

namespace Sierra {
    class MemoryPool {
        public:
            MemoryPool(GLsizeiptr sizeBytes, GLenum usage = GL_DYNAMIC_DRAW);
            ~MemoryPool();

            //Returns an offset into the buffer or nullopt if allocation fails
            std::optional<GLintptr> allocate(GLsizeiptr size, GLsizeiptr alignment = 256);
            void free(GLintptr offset, GLsizeiptr size);

            GLuint getBufferID() const { return mBufferID; }
        
        private:
            struct FreeBlock {
                GLintptr offset;
                GLsizeiptr size;
            };

            void mMergeFreeBlocks();

            GLuint mBufferID;
            GLsizeiptr mPoolSize;
            std::vector<FreeBlock> mFreeBlocks;
    };
}