#pragma once

#include <glad/glad.h>

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <stdfloat>
#include <optional>

#include "gl/util/vao/gl_vao.hpp"
#include "gl/util/gl_util.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "scene/object/2d/object2d.hpp"
#include "util/utilities.hpp"
#include "io/logging/logger.hpp"
#include "scene/ref/resource_ref.hpp"

namespace Sierra {

    typedef std::unordered_map<ResourceRef<Object2D>, GLuint> ObjectMap;

    class RenderBatch2D {
        friend class Renderer2D;
        public:
            RenderBatch2D() = default;
            RenderBatch2D(size_t instanceDataSize, size_t instanceDataCount);

        protected: 

            void addObject(ResourceRef<Object2D> object, GLuint texture);
            std::optional<ObjectMap::iterator> removeObject(ResourceRef<Object2D> object, GLuint texture);
            
            void cleanup();
            bool shouldCleanup();

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

            static const uint32_t BATCH_CLEARUP_TRESHOLD_MEMORY = 65536; // 64kb
            static const uint32_t BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT = 4;

        private:
            void initBuffers();
    };

  
}