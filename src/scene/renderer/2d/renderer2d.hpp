#pragma once

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cstring>
#include <stdfloat>

#include <glad/glad.h>

#include "../../object/2d/object2D.hpp"

#define RENDERER2D_BATCH_CLEARUP_TRESHOLD 65536 // 64kb

namespace GMTKEngine {

    struct RenderBatch2D {
        std::unordered_map<GLuint, std::unordered_map<Object2D*, size_t>> objects;

        std::vector<std::float32_t> objectData;
        std::vector<size_t> clearQueue;

        size_t instanceCount;
        size_t instanceDataSize;
    };

    class Renderer2D {
        public:
            void render();

            void addObject2d(Object2D* object);
            void removeObject2d(Object2D* object);
        private:
            void appendObjectToBatch(RenderBatch2D& batch, Object2D* object);

            void cleanupBatch(RenderBatch2D& batch);
            void cleanupBatchLarge(RenderBatch2D& batch);
            void cleanupBatchSmall(RenderBatch2D& batch);

            /*
            hash is made by combining the shader program id and 
            the type hash so we can know what can be grouped together,
             will be reordered as needeed
            */ 

            //think of this as a map, with all the hashes that as a value, has
            //a vector of objects each with different sprites batched in groups of 32
            //(the max OpenGL allows the bound at once)
            std::unordered_map<GLuint, std::vector<RenderBatch2D>> draw_batches_2d;
    };
}