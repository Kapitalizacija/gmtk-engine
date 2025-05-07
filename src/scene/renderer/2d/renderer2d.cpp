#include "renderer2d.hpp"


namespace GMTKEngine {
    void Renderer2D::addObject2d(Object2D* object) {
        auto& object_group = draw_batches_2d[object->program];
        
        bool found_group = false;
        for (auto& tex_group : object_group) {
            if (tex_group.find(object->mTextureID) == tex_group.end() && tex_group.size() >= 32) {
                continue;
            } 

            RenderBatch2D& batch = tex_group[object->mTextureID];
            appendObjectToBatch(batch, object);

            found_group = true;

            break;
        }

        if (!found_group) {
            object_group.emplace_back();

            RenderBatch2D& batch = object_group[object_group.size() - 1][object->mTextureID];
            batch.instanceCount = 0;
            batch.instanceDataSize = sizeof(std::float32_t) * object->getDrawData().size();

            appendObjectToBatch(batch, object);
        }

        object->rendered = true;
    }
    
    void Renderer2D::removeObject2d(Object2D* object) {
        bool found = false;

        auto& object_group = draw_batches_2d[object->program];
        for (auto& tex_group : object_group) {

            if (tex_group.find(object->mTextureID) != tex_group.end()) {
                RenderBatch2D& batch = tex_group[object->mTextureID];

                batch.clearQueue.push_back(batch.objects[object]);
                batch.objects.erase(object);

                if (batch.clearQueue.size() * batch.instanceDataSize > RENDERER2D_BATCH_CLEARUP_TRESHOLD
                    || batch.clearQueue.size() >= 64) {
                    cleanupBatch(batch);
                }
                
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
        batch.objects[object] = batch.instanceCount;
        batch.instanceCount++;

        object->rendered = true;

        std::vector<float> new_dat = object->getDrawData();
        batch.objectData.insert(batch.objectData.end(), new_dat.begin(), new_dat.end());

        if (batch.clearQueue.size() * batch.instanceDataSize >= RENDERER2D_BATCH_CLEARUP_TRESHOLD) {
            cleanupBatch(batch);
        }
    }
    
    void Renderer2D::cleanupBatch(RenderBatch2D& batch) {
        if (batch.clearQueue.size() > 2) {
            cleanupBatchLarge(batch);
        } else {
            cleanupBatchSmall(batch);
        }

        batch.objectData.erase(batch.objectData.end() - (batch.clearQueue.size() * batch.instanceDataSize / sizeof(std::float32_t)), batch.objectData.end());

        batch.instanceCount -= batch.clearQueue.size();
        batch.clearQueue.clear();
    }
    
    void Renderer2D::cleanupBatchLarge(RenderBatch2D& batch) {
        std::sort(batch.clearQueue.begin(), batch.clearQueue.end());

        size_t dstStartOffset = batch.clearQueue[0] * batch.instanceDataSize;
        for ( size_t i = 1; i < batch.clearQueue.size() - 2; i += 3) {
            size_t dstEndOffset = batch.clearQueue[i] * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue[i+1] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.clearQueue[i+2] * batch.instanceDataSize;

            if (srcEndOffset - srcStartOffset == 0) {
                continue;
            }

            memcpy(batch.objectData.data() + dstEndOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);

            dstStartOffset = dstEndOffset + (srcEndOffset - srcStartOffset);
        }

        if ( (batch.clearQueue.size() - 1) % 3 == 1 ) {
            // 2 elements remain
            size_t dstEndOffset = batch.clearQueue[batch.clearQueue.size() - 2] * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue[batch.clearQueue.size() - 1] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = (batch.clearQueue.size() - 1) * sizeof(std::float32_t);

            if (srcEndOffset - srcStartOffset == 0) {
               return; 
            }

            memcpy(batch.objectData.data() + dstEndOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);

        } else {
            // 1 elements remains
            size_t dstEndOffset = batch.clearQueue.back() * batch.instanceDataSize;

            size_t srcStartOffset = batch.clearQueue.back() * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.objectData.size() * sizeof(std::float32_t);

            if (srcEndOffset - srcStartOffset == 0) {
               return; 
            }

            memcpy(batch.objectData.data() + dstEndOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);
        }
    }
    
    void Renderer2D::cleanupBatchSmall(RenderBatch2D& batch) {
        for (size_t i = 0; i < batch.clearQueue.size(); i++ ) {
            size_t start_offset = batch.clearQueue[i] * batch.instanceDataSize / sizeof(std::float32_t);

            batch.objectData.erase(
                batch.objectData.begin() + start_offset,
                batch.objectData.begin() + start_offset + (batch.instanceDataSize / sizeof(std::float32_t))
            );
        }
    }
}