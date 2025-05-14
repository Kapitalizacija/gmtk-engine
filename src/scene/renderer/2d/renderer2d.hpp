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
#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../object/2d/object2d.hpp"
#include "../../object/camera/camera.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/vao/gl_vao.hpp"

#define RENDERER2D_BATCH_CLEARUP_TRESHOLD_MEMORY 65536 // 64kb
#define RENDERER2D_BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT 128



typedef uint32_t bool32_t;

namespace GMTKEngine {
    
    struct WeakPtrObjectHash {
        std::size_t operator()(const std::weak_ptr<Object2D> weakPtr) const noexcept {
            return std::hash<std::shared_ptr<Object2D>>{}(weakPtr.lock());
        }
    };
    
    struct WeakPtrObjectEquals {
        std::size_t operator()(const std::weak_ptr<Object2D> lhs, const std::weak_ptr<Object2D> rhs) const noexcept {
            return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
        }
    };

    typedef std::unordered_map<std::weak_ptr<Object2D>, GLuint, WeakPtrObjectHash, WeakPtrObjectEquals> ObjectMap;
    
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

            Renderer2D(Renderer2D&& yes) = delete;
            Renderer2D(Renderer2D& yes) = delete;

            void render(Camera& camera);
            void update();

            void addObject2d(std::weak_ptr<Object2D> object);
            std::optional<ObjectMap::iterator> removeObject2d(std::weak_ptr<Object2D> object, GLuint oldTex = 0);

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
    };
}