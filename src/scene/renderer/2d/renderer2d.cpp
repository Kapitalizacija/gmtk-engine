#include "renderer2d.hpp"

#define SHOULD_CLEANUP(batch) batch.clearQueue.size() * batch.instanceDataSize > BATCH_CLEARUP_TRESHOLD_MEMORY \
                    || batch.clearQueue.size() >= BATCH_CLEARUP_TRESHOLD_OBJECT_COUNT

const uint32_t indices[] = { // the most barebones indices needed for a square, therefore I feel comfortable making it a global cosntant
    0, 1, 2,
    1, 2, 3
};

const std::float32_t vertices[] = {
    -0.5f, -0.5f,
    -0.5f,  0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f    
};

namespace GMTKEngine {
    Renderer2D::Renderer2D() {
        createGLBuffers();
    }

    void Renderer2D::createGLBuffers(){
        ebo = GLBuffer(GLBuffer::Type::INDEX, indices, sizeof(indices), GLBuffer::Usage::RARELY);
        vbo = GLBuffer(GLBuffer::Type::VERTEX, vertices, sizeof(vertices), GLBuffer::Usage::RARELY);
    }

    void Renderer2D::update() {
        for (std::list<std::thread>::iterator it = cleanupThreads.begin(); it != cleanupThreads.end();) {
            if (it->joinable()) {
                it->join();

                auto old_it = it;
                it++;
                cleanupThreads.erase(old_it);
                continue;
            }
            it++;
        }
    }

    void Renderer2D::render(Camera& camera) {
        updateInstanceData();

        GLint texture_indices[32];
        for (size_t i = 0; i < 32; i++) {
            texture_indices[i] = i;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (auto& shaderGroup : draw_batches_2d) {
            glUseProgram(shaderGroup.first);

            GLint texturesUniformLoc = glGetUniformLocation(shaderGroup.first, "textures");
            GLuint projectionLoc = glGetUniformLocation(shaderGroup.first, "projection");

            camera.applyProjection(projectionLoc);

            glUniform1iv(texturesUniformLoc, 32, texture_indices);


            for (RenderBatch2D& batch : shaderGroup.second) {
                std::lock_guard lock(*batch.cleanupMutex.get());

                std::vector<uint32_t> textureOffsets;
                textureOffsets.resize(32);

                batch.objectDataGLBuffer.upload_data(batch.objectData.data(), batch.objectData.size() * sizeof(std::float32_t), GLBuffer::Usage::OFTEN);
                batch.extraDrawDataGLBuffer.upload_data(batch.extraDrawInfo.data(), batch.extraDrawInfo.size() * sizeof(bool32_t), GLBuffer::Usage::OFTEN);
                
                auto it = batch.objects.begin();
                for ( size_t i = 0; i < batch.objects.size(); i++) {
                    if ( it->second.size() == 0 ) {
                        it++;
                        continue;
                    }

                    size_t texIndex = batch.textureInsertionIndex[it->first];

                    glActiveTexture(GL_TEXTURE0 + texIndex);
                    glBindTexture(GL_TEXTURE_2D, it->first);
                    it++;
                }

                glBindVertexArray(batch.vao.getVAO());

                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, batch.instanceCount);
            }
        }

        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Renderer2D::addObject2d(std::weak_ptr<Object2D> object) {
        std::shared_ptr<Object2D> shared = object.lock();

        auto& shader_group = draw_batches_2d[shared->program];
        
        GLuint textureID = (shared->getComponentLock<Texture>()).value()->getRawHandle();

        bool found_group = false;
        for (auto& tex_group : shader_group) {
            auto it = tex_group.objects.find(textureID);
            if (it == tex_group.objects.end()) {
                if(tex_group.objects.size() >= 32) {
                    continue;
                }

                tex_group.textureInsertionIndex[textureID] = tex_group.textureInsertionIndex.size();
            } 

            
            RenderBatch2D& batch = tex_group;

            std::lock_guard lock(*batch.cleanupMutex.get());

            appendObjectToBatch(batch, object, shared);

            found_group = true;

            break;
        }

        if (!found_group) {
            shader_group.emplace_back();

            RenderBatch2D& batch = shader_group.back();
            batch.instanceCount = 0;
            batch.instanceDataSize = sizeof(std::float32_t) * shared->getDrawData().size();
            batch.instanceDataCount = shared->getDrawData().size();
            batch.cleanupMutex = std::make_shared<std::mutex>();

            initBatch(batch);

            batch.textureInsertionIndex[textureID] = batch.textureInsertionIndex.size();

            appendObjectToBatch(batch, object, shared);
        }

        shared->rendered = true;
    }
    
    std::optional<ObjectMap::iterator> Renderer2D::removeObject2d(std::weak_ptr<Object2D> object, GLuint oldTex) {
        bool found = false;

        std::shared_ptr<Object2D> shared = object.lock();

        GLuint textureID = (shared->getComponentLock<Texture>()).value()->getRawHandle();

        if (oldTex != 0) {
            textureID = oldTex;
        }

        ObjectMap::iterator nextIt;

            
        auto& shader_group = draw_batches_2d[shared->program];
        for (auto& batch : shader_group) {

            auto texIt = batch.objects.find(textureID);
            if (texIt != batch.objects.end())   {
                
                auto objectIt = texIt->second.find(object);
                if (objectIt == texIt->second.end()) {
                    ERROR("Tried to remove nonexistant object: " << shared.get());
                    return std::nullopt;
                }

                std::lock_guard lock(*batch.cleanupMutex.get());
         
                batch.clearQueue.push_back(objectIt->second);

                batch.extraDrawInfo[objectIt->second * 2] = false;

                nextIt = texIt->second.erase(objectIt);

                if (oldTex != 0) {
                    shared->rendered = false;
                }

                if (SHOULD_CLEANUP(batch)) {
                    cleanupBatch(batch);
                }
                found = true;

                break;
            }
        }

        if (!found) {
            ERROR("Tried to remove nonexistant object");
            return nextIt;
        }

        return nextIt;

    }
    
    void Renderer2D::appendObjectToBatch(RenderBatch2D& batch, std::weak_ptr<Object2D> object, std::shared_ptr<Object2D> shared) {

        GLuint textureID = (shared->getComponentLock<Texture>()).value()->getRawHandle();

        batch.objects[textureID][object] = batch.instanceCount;
        
        batch.extraDrawInfo.push_back(true);
        batch.extraDrawInfo.push_back(batch.textureInsertionIndex[textureID]);
        batch.instanceCount++;

        shared->rendered = true;

        std::vector<float> new_dat = shared->getDrawData();
        batch.objectData.insert(batch.objectData.end(), new_dat.begin(), new_dat.end());

        if (SHOULD_CLEANUP(batch)) {
            cleanupBatch(batch);
        }
    }
    
    void Renderer2D::cleanupBatch(RenderBatch2D& batch) {
        auto cleanup = [](Renderer2D* renderer, RenderBatch2D& batch) {
            std::lock_guard lock(*batch.cleanupMutex.get());

            if (batch.clearQueue.size() > 3) {
                renderer->cleanupBatchLarge(batch);

                batch.objectData.erase(batch.objectData.end() - batch.clearQueue.size() * batch.instanceDataCount, batch.objectData.end());
                batch.extraDrawInfo.erase(batch.extraDrawInfo.end() - batch.clearQueue.size() * 2, batch.extraDrawInfo.end());
            } else {
                renderer->cleanupBatchSmall(batch);
            }

            batch.instanceCount -= batch.clearQueue.size();

            batch.clearQueue.clear();

            std::map<size_t, ObjectMap::iterator, std::less<GLuint>> indices;

            for (auto texIt = batch.objects.begin(); texIt != batch.objects.end(); texIt++) {
                for (auto objIt = texIt->second.begin(); objIt != texIt->second.end(); objIt++) {
                    indices[objIt->second] = objIt;
                }
            }

            size_t i = 0;
            for (auto& index : indices) {
                index.second->second = i;
                i++;
            }
        };

        cleanupThreads.emplace_back(cleanup, this, std::ref(batch));
    }
    
    void Renderer2D::cleanupBatchLarge(RenderBatch2D& batch) {
        std::sort(batch.clearQueue.begin(), batch.clearQueue.end());

        size_t dstStartOffset = batch.clearQueue[0];
        for ( size_t i = 1; i < batch.clearQueue.size() - 1; i += 2) {
            size_t srcStartOffset = batch.clearQueue[i] + 1;
            size_t srcEndOffset = batch.clearQueue[i+1];

            
            if (srcEndOffset - srcStartOffset == 0) {
                dstStartOffset += (srcEndOffset - srcStartOffset);
                continue;
            }

            std::copy(batch.objectData.begin() + srcStartOffset * batch.instanceDataCount, batch.objectData.begin() + srcEndOffset * batch.instanceDataCount, batch.objectData.begin() + dstStartOffset *  batch.instanceDataCount);
            std::copy(batch.extraDrawInfo.begin() + srcStartOffset * 2, batch.extraDrawInfo.begin() + srcEndOffset * 2, batch.extraDrawInfo.begin() + dstStartOffset *  2);
            
            dstStartOffset += (srcEndOffset - srcStartOffset);
        }

        size_t srcStartOffset = batch.clearQueue.back() + 1;

        if (srcStartOffset * 2 + 2 == batch.extraDrawInfo.size()) {
           return; 
        }

        std::copy(batch.objectData.begin() + srcStartOffset * batch.instanceDataCount, batch.objectData.end(), batch.objectData.begin() + dstStartOffset *  batch.instanceDataCount);
        std::copy(batch.extraDrawInfo.begin() + srcStartOffset * 2, batch.extraDrawInfo.end(), batch.extraDrawInfo.begin() + dstStartOffset *  2);
    }
    
    void Renderer2D::cleanupBatchSmall(RenderBatch2D& batch) {
        for (size_t i = 0; i < batch.clearQueue.size(); i++ ) {
            size_t startOffset = batch.clearQueue[i] * batch.instanceCount;

            batch.objectData.erase(
                batch.objectData.begin() + startOffset,
                batch.objectData.begin() + startOffset + batch.instanceCount
            );

            batch.extraDrawInfo.erase(
                batch.extraDrawInfo.begin() + batch.clearQueue[i] * 2,
                batch.extraDrawInfo.begin() + batch.clearQueue[i] * 2 + 2
            );

        }
    }

    void Renderer2D::updateInstanceData() { 
        for (auto& shaderGroup : draw_batches_2d) {
            for(RenderBatch2D& batch : shaderGroup.second) {
                for (auto& textureGroup : batch.objects) {
                    for (auto it = textureGroup.second.begin(); it != textureGroup.second.end();) {
                        if ( (*it).first.lock()->changedComponent<Texture>()) {
                            std::shared_ptr<Object2D> shared = (*it).first.lock();

                            it = removeObject2d((*it).first, textureGroup.first).value();

                            addObject2d((*it).first);
                            
                            shared->getComponentLock<Texture>().value()->frameCleanup();
                            continue;
                        }

                        // I lock it in the if statement intentionally so no deadlock 
                        if ( !(*it).first.lock()->changedComponent<Transform2D>() ) {
                            it++;
                            continue;
                        }

                        std::vector<float> newData = (*it).first.lock()->getDrawData();
                        memcpy((char*)batch.objectData.data() + (*it).second * batch.instanceDataSize, newData.data(), batch.instanceDataSize); 

                        it++;
                    }
                }
            }
        }
    }



    void Renderer2D::initBatch(RenderBatch2D& batch) {
        batch.objectDataGLBuffer = GLBuffer(GLBuffer::Type::VERTEX);
        batch.extraDrawDataGLBuffer = GLBuffer(GLBuffer::Type::VERTEX);

        GLAttribPointer ptr0;
        ptr0.buff = &vbo;
        ptr0.index = 0;
        ptr0.componentCount = 2;
        ptr0.type = GL_FLOAT;
        ptr0.stride = sizeof(std::float32_t) * 2;
        ptr0.offset = 0;
        ptr0.isInstanced = false;

        GLAttribPointer ptr1;
        ptr1.buff = &batch.objectDataGLBuffer;
        ptr1.index = 1;
        ptr1.componentCount = 2;
        ptr1.type = GL_FLOAT;
        ptr1.stride = batch.instanceDataSize;
        ptr1.offset = 0;
        ptr1.isInstanced = true;

        GLAttribPointer ptr2;
        ptr2.buff = &batch.objectDataGLBuffer;
        ptr2.index = 2;
        ptr2.componentCount = 1;
        ptr2.type = GL_FLOAT;
        ptr2.stride = batch.instanceDataSize;
        ptr2.offset = 8;
        ptr2.isInstanced = true;

        GLAttribPointer ptr3;
        ptr3.buff = &batch.objectDataGLBuffer;
        ptr3.index = 3;
        ptr3.componentCount = 1;
        ptr3.type = GL_FLOAT;
        ptr3.stride = batch.instanceDataSize;
        ptr3.offset = 12;
        ptr3.isInstanced = true;

        GLAttribPointer ptr4;
        ptr4.buff = &batch.objectDataGLBuffer;
        ptr4.index = 4;
        ptr4.componentCount = 2;
        ptr4.type = GL_FLOAT;
        ptr4.stride = batch.instanceDataSize;
        ptr4.offset = 16;
        ptr4.isInstanced = true;

        GLAttribPointer ptr5;
        ptr5.buff = &batch.extraDrawDataGLBuffer;
        ptr5.index = 5;
        ptr5.componentCount = 1;
        ptr5.type = GL_INT;
        ptr5.stride = sizeof(bool32_t) * 2;
        ptr5.offset = 0;
        ptr5.isInstanced = true;

        GLAttribPointer ptr6;
        ptr6.buff = &batch.extraDrawDataGLBuffer;
        ptr6.index = 6;
        ptr6.componentCount = 1;
        ptr6.type = GL_INT;
        ptr6.stride = sizeof(bool32_t) * 2;
        ptr6.offset = 4;
        ptr6.isInstanced = true;

        GLAttribPointer ptrs[] = {ptr0, ptr1, ptr2, ptr3, ptr4, ptr5, ptr6};

        batch.vao = GLVAO(ptrs, sizeof(ptrs) / sizeof(GLAttribPointer));
        batch.vao.addEBO(ebo);
    }
    
    void Renderer2D::freeUnusedMemory() {
        for (auto& shader_group : draw_batches_2d) {
            for (RenderBatch2D& batch : shader_group.second) {
                cleanupBatch(batch);
            }
        }
    }
}