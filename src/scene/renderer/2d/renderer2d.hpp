#pragma once


#include <unordered_map>
#include <map>
#include <algorithm>
#include <vector>
#include <list>
#include <cstring>
#include <thread>
#include <mutex>
#include <stdfloat>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../object/2d/object2d.hpp"
#include "../../object/camera/camera.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/vao/gl_vao.hpp"

#define RENDERER2D_BATCH_CLEARUP_TRESHOLD_MEMORY 65536 // 64kb
#define RENDERER2D_BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT 256

typedef uint32_t bool32_t;

namespace GMTKEngine {

    struct RenderBatch2D {
        std::shared_ptr<std::mutex> cleanupMutex;

        std::map<GLuint, std::unordered_map<Object2D*, GLuint>> objects;
        std::unordered_map<GLuint, size_t> textureInsertionIndex;

        std::vector<std::float32_t> objectData;
        std::vector<int32_t> extraDrawInfo;

        std::vector<size_t> clearQueue;

        size_t instanceCount;
        size_t instanceDataSize;

        GLVAO vao;
        GLBuffer objectDataGLBuffer;
        GLBuffer extraDrawDataGLBuffer;
    };

    class Renderer2D {
        public:
            Renderer2D();

            Renderer2D(Renderer2D&& yes) = delete;
            Renderer2D(Renderer2D& yes) = delete;

            void render(Camera& camera);
            void update();

            void addObject2d(Object2D* object);
            std::unordered_map<Object2D*, GLuint>::iterator removeObject2d(Object2D* object, GLuint oldTex = 0);

            void freeUnusedMemory();
        private:
            void createGLBuffers();

            void appendObjectToBatch(RenderBatch2D& batch, Object2D* object);

            void cleanupBatch(RenderBatch2D& batch);
            void cleanupBatchLarge(RenderBatch2D& batch);
            void cleanupBatchSmall(RenderBatch2D& batch);

            void initBatch(RenderBatch2D& batch);

            void updateInstanceData();

            GLBuffer ebo;
            GLBuffer vbo;
            
            std::unordered_map<GLuint, std::vector<RenderBatch2D>> draw_batches_2d;

            std::list<std::thread> cleanupThreads;
    };
}