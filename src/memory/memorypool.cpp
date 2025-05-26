#include "memorypool.hpp"

namespace Sierra {
    //This might have to be rewritten
    //This was written with limited knowledge of OpenGL, kweezo should look over it and make the much needed changes.

    MemoryPool::MemoryPool(GLsizeiptr sizeBytes, GLenum usage)
        : mPoolSize(sizeBytes)
    {
        glGenBuffers(1, &mBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
        glBufferData(GL_ARRAY_BUFFER, mPoolSize, nullptr, usage);

        mFreeBlocks.push_back({ 0, mPoolSize });
    }

    MemoryPool::~MemoryPool() {
        glDeleteBuffers(1, &mBufferID);
    }

    std::optional<GLintptr> MemoryPool::allocate(GLsizeiptr size, GLsizeiptr alignment) {
        //This is some dogshit I cobbled together with my limited knowledge and Chatto
        for (auto it = mFreeBlocks.begin(); it != mFreeBlocks.end(); ++it) {
            GLintptr alignedOffset = (it->offset + alignment - 1) & ~(alignment - 1); //What the fuck?
            GLsizeiptr padding = alignedOffset - it->offset;

            if (it->size >= size + padding) {
                if (padding > 0) {
                    //Adjust to exclude the padding
                    it->size -= padding;
                    it->offset += padding;
                }

                GLintptr resultOffset = it->offset;
                it->offset += size;
                it->size -= size;

                if (it->size == 0) {
                    mFreeBlocks.erase(it); //gtfo
                }

                return resultOffset;
            }
        }

        return std::nullopt;
    }

    void MemoryPool::free(GLintptr offset, GLsizeiptr size) {
        mFreeBlocks.push_back({ offset, size });
        mMergeFreeBlocks();
    }

    void MemoryPool::mMergeFreeBlocks() {
        std::sort(mFreeBlocks.begin(), mFreeBlocks.end(), [](const FreeBlock& a, const FreeBlock& b) {
            return a.offset < b.offset;
        });
    
        std::vector<FreeBlock> merged;
        for (const auto& block : mFreeBlocks) {
            if (!merged.empty() && merged.back().offset + merged.back().size == block.offset) {
                merged.back().size += block.size;
            } else {
                merged.push_back(block);
            }
        }
        mFreeBlocks = std::move(merged);
    }
}