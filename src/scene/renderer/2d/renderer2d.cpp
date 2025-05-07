#include "renderer2d.hpp"


namespace GMTKEngine {

    void Renderer2D::render() {
        for (auto& shader_group : draw_batches_2d) {
            glUseProgram(shader_group.first);

            for (RenderBatch2D& batch : shader_group.second) {
                batch.batchData.upload_data(batch.objectData.data(), batch.objectData.size() * sizeof(std::float32_t), GLBuffer::OFTEN);

                auto it = batch.objects.begin();
                for ( size_t i = 0; i < batch.objects.size(); i++) {
                    glBindTexture(GL_TEXTURE0 + i, it->first);
                    it++;
                }

                glBindVertexArray(batch.vao.getVAO());

                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, batch.instanceCount);
            }
        }

        for (size_t i = 0; i < 32; i++) {
            glBindTexture(GL_TEXTURE0 + i, 0);
        }
        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Renderer2D::addObject2d(Object2D* object) {
        auto& shader_group = draw_batches_2d[object->program];
        
        bool found_group = false;
        for (auto& tex_group : shader_group) {
            if (tex_group.objects.find(object->mTextureID) == tex_group.objects.end() && tex_group.objects.size() >= 32) {
                continue;
            } 

            RenderBatch2D& batch = tex_group;
            appendObjectToBatch(batch, object);

            found_group = true;

            break;
        }

        if (!found_group) {
            shader_group.emplace_back();

            RenderBatch2D& batch = shader_group.back();
            batch.instanceCount = 0;
            batch.instanceDataSize = sizeof(std::float32_t) * object->getDrawData().size();

            initBatch(batch);

            appendObjectToBatch(batch, object);
        }

        object->rendered = true;
    }
    
    void Renderer2D::removeObject2d(Object2D* object) {
        bool found = false;

        auto& shader_group = draw_batches_2d[object->program];
        for (auto& batch : shader_group) {

            if (batch.objects.find(object->mTextureID) != batch.objects.end()) {
                RenderBatch2D& batch = batch;

                batch.clearQueue.push_back(batch.objects[object->mTextureID][object]);
                batch.objects[object->mTextureID].erase(object);

                if (batch.clearQueue.size() * batch.instanceDataSize > RENDERER2D_BATCH_CLEARUP_TRESHOLD
                    || batch.clearQueue.size() >= 64) {
                    cleanupBatch(batch);
                }
                object->rendered = false;
                
                found = true;
                break;
            }
        }

        if (!found) {
            ERROR("Tried to remove nonexistant object");
            return;
        }

    }
    
    void Renderer2D::appendObjectToBatch(RenderBatch2D& batch, Object2D* object) {
        batch.objects[object->mTextureID][object] = batch.instanceCount;
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

        for (auto& tex_group : batch.objects) {
            for (auto& object : tex_group.second) {
                object.second -= batch.clearQueue.size();
            }
        }

        batch.clearQueue.clear();
    }
    
    void Renderer2D::cleanupBatchLarge(RenderBatch2D& batch) {
        std::sort(batch.clearQueue.begin(), batch.clearQueue.end());

        size_t dstStartOffset = batch.clearQueue[0] * batch.instanceDataSize;
        for ( size_t i = 1; i < batch.clearQueue.size() - 1; i += 3) {
            size_t srcStartOffset = batch.clearQueue[i] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.clearQueue[i+1] * batch.instanceDataSize;

            dstStartOffset += (srcEndOffset - srcStartOffset);

            if (srcEndOffset - srcStartOffset == 0) {
                continue;
            }

            memcpy(batch.objectData.data() + dstStartOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);

        }

        if ( (batch.clearQueue.size() - 1) % 3 == 2 ) {
            // 2 elements remain
            size_t srcStartOffset = batch.clearQueue.back() * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = (batch.clearQueue.size() - 1) * sizeof(std::float32_t);

            if (srcEndOffset - srcStartOffset == 0) {
               return; 
            }

            memcpy(batch.objectData.data() + dstStartOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);

        } else {
            // 1 elements remains
            size_t srcStartOffset = batch.clearQueue.back() * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.objectData.size() * sizeof(std::float32_t);

            if (srcEndOffset - srcStartOffset == 0) {
               return; 
            }

            DBG(dstStartOffset << " " << " " << srcEndOffset << " " << srcStartOffset << " " << batch.objectData.size() * 4);
            memcpy(batch.objectData.data() + dstStartOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);
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

    void Renderer2D::initBatch(RenderBatch2D& batch) {
        batch.batchData = GLBuffer(GLBuffer::VERTEX);

        GLAttribPointer ptr1;
        ptr1.buff = &batch.batchData;
        ptr1.component_count = 3;
        ptr1.type = GL_FLOAT;
        ptr1.stride = batch.instanceDataSize;
        ptr1.offset = 0;

        GLAttribPointer ptr2;
        ptr2.buff = &batch.batchData;
        ptr2.component_count = 4;
        ptr2.type = GL_FLOAT;
        ptr2.stride = batch.instanceDataSize;
        ptr2.offset = 12;

        GLAttribPointer ptr3;
        ptr3.buff = &batch.batchData;
        ptr3.component_count = 3;
        ptr3.type = GL_FLOAT;
        ptr3.stride = batch.instanceDataSize;
        ptr3.offset = 28;

        GLAttribPointer ptrs[] = {ptr1, ptr2, ptr3};

        batch.vao = GLVAO(ptrs, 3);
    }
}