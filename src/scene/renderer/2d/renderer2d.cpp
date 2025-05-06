#include "renderer2d.hpp"


namespace GMTKEngine {
    void Renderer2D::addObject2d(Object2D* object) {
        auto& object_group = draw_batches_2d[object->program];
        
        bool found_group = false;
        for (auto& shader_group : object_group) {
            if (shader_group.find(object->mTextureID) == shader_group.end() && shader_group.size() >= 32) {
                continue;
            } 

            RenderBatch2D& batch = object_group[object_group.size() - 1][object->mTextureID];
            appendObjectToBatch(batch, object);

            break;
        }

        if (!found_group) {
            object_group.emplace_back();

            RenderBatch2D& batch = object_group[object_group.size() - 1][object->mTextureID];
            batch.instanceCount = 0;
            batch.instanceDataSize = sizeof(object->getDrawData());

            appendObjectToBatch(batch, object);
        }

        object->rendered = true;
    }
    
    void Renderer2D::removeObject2d(Object2D* object) {
        bool found = false;

        auto& object_group = draw_batches_2d[object->program];
        for (auto& shader_group : object_group) {

            if (shader_group.find(object->mTextureID) != shader_group.end()) {
                RenderBatch2D& batch = shader_group[object->mTextureID];

                batch.clearQueue.push_back(batch.objects[object]);
                batch.objects.erase(object);
                
                found = true;
                break;
            }
        }

        if (!found) {
            ERROR("Tried to remove nonexistant object");
            return;
        }

        object->rendered = false;

    }
    
    void Renderer2D::appendObjectToBatch(RenderBatch2D& batch, Object2D* object) {
        batch.objects[object] = batch.instanceDataSize;
        batch.instanceCount++;

        batch.objectData.insert(batch.objectData.end(), object->getDrawData().begin(), object->getDrawData().end());

        if (batch.clearQueue.size() * batch.instanceDataSize > RENDERER2D_BATCH_CLEARUP_TRESHOLD) {
            cleanupBatch(batch);
        }
    }
    
    void Renderer2D::cleanupBatch(RenderBatch2D& batch) {
    
        if (batch.clearQueue.size() > 3) {
            cleanupBatchLarge(batch);
        } else {
            cleanupBatchSmall(batch);
        }

    }
    
    void Renderer2D::cleanupBatchLarge(RenderBatch2D& batch) {
        size_t dstStartOffset = batch.clearQueue[0] * batch.instanceDataSize;
        for ( size_t i = 1; i < batch.clearQueue.size() - 2; i += 3) {
            size_t dstEndOffset = batch.clearQueue[i] * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue[i+1] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.clearQueue[i+2] * batch.instanceDataSize;

            memcpy(batch.clearQueue.data() + dstEndOffset, batch.clearQueue.data() + srcStartOffset, srcEndOffset - srcStartOffset);

            dstStartOffset = dstEndOffset + (srcEndOffset - srcStartOffset);
        }

        if ( (batch.clearQueue.size() - 1) % 3 == 1 ) {
            // 2 elements remain
            size_t dstEndOffset = batch.clearQueue[batch.clearQueue.size() - 2] * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue[batch.clearQueue.size() - 1] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = (batch.clearQueue.size() - 1) * sizeof(std::float32_t);

            memcpy(batch.clearQueue.data() + dstEndOffset, batch.clearQueue.data() + srcStartOffset, srcEndOffset - srcStartOffset);

        } else {
            // 1 elements remains
            size_t dstEndOffset = batch.clearQueue[batch.clearQueue.size() - 1] * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue[batch.clearQueue.size() - 1] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = (batch.clearQueue.size() - 1) * sizeof(std::float32_t);

            memcpy(batch.clearQueue.data() + dstEndOffset, batch.clearQueue.data() + srcStartOffset, srcEndOffset - srcStartOffset);
        }
    }
    
    void Renderer2D::cleanupBatchSmall(RenderBatch2D& batch) {

    }
}