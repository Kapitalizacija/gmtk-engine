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
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../object/2d/object2d.hpp"
#include "../../object/camera/camera.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/vao/gl_vao.hpp"
#include "util/utilities.hpp"


typedef uint32_t bool32_t;

namespace GMTKEngine {
    

  

    typedef std::unordered_map<std::weak_ptr<Object2D>, GLuint, WeakPtrObjectHash<Object2D>, WeakPtrObjectEquals<Object2D>> ObjectMap;
    
    struct RenderBatch2D {
        std::shared_ptr<std::mutex> cleanupMutex;
        
        std::unordered_map<GLuint, ObjectMap> objects;
        std::unordered_map<GLuint, size_t> textureInsertionIndex;
        
        std::vector<std::float32_t> objectData;
        std::vector<int32_t> extraDrawInfo;

        std::vector<size_t> clearQueue;

        size_t instanceCount;
        size_t instanceDataSize;
        size_t instanceDataCount;

        GLVAO vao;
        GLBuffer objectDataGLBuffer;
        GLBuffer extraDrawDataGLBuffer;
    };

    class Renderer2D {
        public:
            Renderer2D();

            Renderer2D(Renderer2D&&) = delete;
            Renderer2D(Renderer2D&) = delete;

            void render(Camera& camera);
            void update();

            void addObject(std::weak_ptr<Object2D> object);
            ObjectMap::iterator removeObject2d(std::weak_ptr<Object2D> object, GLuint oldTex = 0);

            void freeUnusedMemory();
        private:
            void createGLBuffers();

            void appendObjectToBatch(RenderBatch2D& batch, std::weak_ptr<Object2D> object, std::shared_ptr<Object2D> shared);

            void cleanupBatch(RenderBatch2D& batch);
            void cleanupBatchLarge(RenderBatch2D& batch);
            void cleanupBatchSmall(RenderBatch2D& batch);

            void initBatch(RenderBatch2D& batch);

            void updateInstanceData();

            GLBuffer ebo;
            GLBuffer vbo;
            
            std::unordered_map<GLuint, std::vector<RenderBatch2D>> draw_batches_2d;

            std::list<std::thread> cleanupThreads;

            static const uint32_t BATCH_CLEARUP_TRESHOLD_MEMORY = 65536; // 64kb
            static const uint32_t BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT = 4;
    };
}