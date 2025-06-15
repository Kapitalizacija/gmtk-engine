#include "batch2d.hpp"

#define SHOULD_CLEANUP clearQueue.size() * instanceDataSize > BATCH_CLEARUP_TRESHOLD_MEMORY \
                    || clearQueue.size() >= BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT

namespace Sierra {
    RenderBatch2D::RenderBatch2D(size_t instanceDataSize, size_t instanceDataCount): instanceDataSize(instanceDataSize),
     instanceDataCount(instanceDataCount) {
            instanceCount = 0;
            cleanupMutex = std::make_shared<std::mutex>();

        initBuffers();
    }

    void RenderBatch2D::addObject(ResourceRef<Object2D> object, GLuint texture) {
        GLuint textureID = object->getComponent<Component::Texture>()->getRawHandle();

        objects[textureID][object] = instanceCount;
        
        extraDrawInfo.push_back(true);
        extraDrawInfo.push_back(textureInsertionIndex[textureID]);
        instanceCount++;

        object->rendered = true;

        std::vector<float> new_dat = object->getDrawData();
        objectData.insert(objectData.end(), new_dat.begin(), new_dat.end());

        glm::ivec3 colorVec = object->getComponent<Component::Texture>()->getColor();
        objectData.push_back(colorVec.r);
        objectData.push_back(colorVec.g);
        objectData.push_back(colorVec.b);
    }

    std::optional<ObjectMap::iterator> RenderBatch2D::removeObject(ResourceRef<Object2D> object, GLuint texture) {
        ObjectMap::iterator nextIt;

        auto texIt = objects.find(texture);
        if (texIt == objects.end())   {
            return std::nullopt;
        }
            
        auto objectIt = texIt->second.find(object);
        if (objectIt == texIt->second.end()) {
            ERROR("Tried to remove nonexistant object: " << object.getHash());
            return {};
        }

        std::lock_guard lock(*cleanupMutex.get());
         
        clearQueue.push_back(objectIt->second);

        extraDrawInfo[objectIt->second * 2] = false;

        nextIt = texIt->second.erase(objectIt);

        if (texture != 0) {
            object->rendered = false;
        }

        return nextIt;
    }

    void RenderBatch2D::initBuffers() {
        objectDataGLBuffer = GLBuffer(GLBuffer::Type::VERTEX);
        extraDrawDataGLBuffer = GLBuffer(GLBuffer::Type::VERTEX);

        GLAttribPointer ptr0;
        ptr0.buff = &GLUtils::getSquareVertexBuffer();
        ptr0.index = 0;
        ptr0.componentCount = 2;
        ptr0.type = GL_FLOAT;
        ptr0.stride = sizeof(std::float32_t) * 2;
        ptr0.offset = 0;
        ptr0.isInstanced = false;

        GLAttribPointer ptr1;
        ptr1.buff = &objectDataGLBuffer;
        ptr1.index = 1;
        ptr1.componentCount = 3;
        ptr1.type = GL_FLOAT;
        ptr1.stride = instanceDataSize;
        ptr1.offset = 0;
        ptr1.isInstanced = true;

        GLAttribPointer ptr2;
        ptr2.buff = &objectDataGLBuffer;
        ptr2.index = 2;
        ptr2.componentCount = 1;
        ptr2.type = GL_FLOAT;
        ptr2.stride = instanceDataSize;
        ptr2.offset = 12;
        ptr2.isInstanced = true;

        GLAttribPointer ptr3;
        ptr3.buff = &objectDataGLBuffer;
        ptr3.index = 3;
        ptr3.componentCount = 2;
        ptr3.type = GL_FLOAT;
        ptr3.stride = instanceDataSize;
        ptr3.offset = 16;
        ptr3.isInstanced = true;

        GLAttribPointer ptr4;
        ptr4.buff = &objectDataGLBuffer;
        ptr4.index = 4;
        ptr4.componentCount = 3;
        ptr4.type = GL_UNSIGNED_INT;
        ptr4.stride = instanceDataSize;
        ptr4.offset = 24;
        ptr4.isInstanced = true;

        GLAttribPointer ptr5;
        ptr5.buff = &extraDrawDataGLBuffer;
        ptr5.index = 5;
        ptr5.componentCount = 1;
        ptr5.type = GL_INT;
        ptr5.stride = sizeof(uint32_t) * 2;
        ptr5.offset = 0;
        ptr5.isInstanced = true;

        GLAttribPointer ptr6;
        ptr6.buff = &extraDrawDataGLBuffer;
        ptr6.index = 6;
        ptr6.componentCount = 1;
        ptr6.type = GL_INT;
        ptr6.stride = sizeof(uint32_t) * 2;
        ptr6.offset = 4;
        ptr6.isInstanced = true;

        GLAttribPointer ptrs[] = {ptr0, ptr1, ptr2, ptr3, ptr4, ptr5, ptr6};

        vao = GLVAO(ptrs, sizeof(ptrs) / sizeof(GLAttribPointer));
        vao.addEBO(GLUtils::getSquareIndexBuffer());
    }

    bool RenderBatch2D::shouldCleanup() {
        return SHOULD_CLEANUP;
    }

}