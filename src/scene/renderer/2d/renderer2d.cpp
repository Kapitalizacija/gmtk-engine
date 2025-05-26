#include "renderer2d.hpp"


namespace Sierra {
    Renderer2D::Renderer2D() {
        createGLBuffers();
    }

    void Renderer2D::createGLBuffers(){
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



        for (auto& shaderGroup : draw_batches_2d) {
            glUseProgram(shaderGroup.first);

            GLint texturesUniformLoc = glGetUniformLocation(shaderGroup.first, "textures");
            GLuint projectionLoc = glGetUniformLocation(shaderGroup.first, "projection");

            camera.__applyProjection(projectionLoc);

            glUniform1iv(texturesUniformLoc, 32, texture_indices);


            for (RenderBatch2D& batch : shaderGroup.second) {
                std::lock_guard lock(*batch.cleanupMutex.get());

                std::vector<uint32_t> textureOffsets;
                textureOffsets.resize(32);

                
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

    void Renderer2D::addObject(ResourceRef<Object2D> object) {

        if (object->program == 0) {
            ERROR("Failed to add object2D to the renderer as the shader is not set: " << object.getLock().get());
            return;
        } 

        auto& shader_group = draw_batches_2d[object->program];
        
        GLuint textureID = object->getComponent<Component::Texture>()->getRawHandle();

        if (textureID == 0) {
            ERROR("Failed to add object2D to the renderer as the texture is invalid: " << object.getLock().get());
            return;
        }

        bool found_group = false;
        for (auto& texGroup : shader_group) {
            auto it = texGroup.objects.find(textureID);
            if (it == texGroup.objects.end()) {
                if(texGroup.objects.size() >= 32) {
                    continue;
                }

                texGroup.textureInsertionIndex[textureID] = texGroup.textureInsertionIndex.size();
            } 
            
            RenderBatch2D& batch = texGroup;

            std::lock_guard lock(*batch.cleanupMutex.get());

            batch.addObject(object, textureID);

            found_group = true;

            break;
        }

        if (!found_group) {
            shader_group.emplace_back(object->getDrawData().size() * sizeof(std::float32_t), object->getDrawData().size());

            RenderBatch2D& batch = shader_group.back();

            batch.textureInsertionIndex[textureID] = batch.textureInsertionIndex.size();

            batch.addObject(object, textureID);
        }

        object->rendered = true;
    }
    
    ObjectMap::iterator Renderer2D::removeObject2d(ResourceRef<Object2D> object, GLuint oldTex) {

        GLuint textureID = object->getComponent<Component::Texture>()->getRawHandle();

        if (oldTex != 0) {
            textureID = oldTex;
        }

        std::optional<ObjectMap::iterator> nextIt;

            
        auto& shader_group = draw_batches_2d[object->program];
        for (auto& batch : shader_group) {
            nextIt = batch.removeObject(object, textureID);
        }

        if (!nextIt.has_value()) {
            ERROR("Tried to remove nonexistant object");
            return {};
        }

        return nextIt.value();

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
                std::vector<GLBufferUpdateRegion> objectDataUpdateRegions;
                std::vector<GLBufferUpdateRegion> extraDataUpdateRegions;

                bool updatedObjectData = false;
                if (batch.objectData.size() * sizeof(std::float32_t) > batch.objectDataGLBuffer.getSize()) {
                    batch.objectDataGLBuffer.uploadData(batch.objectData.data(), batch.objectData.size() * sizeof(std::float32_t), GLBuffer::Usage::OFTEN); 
                    batch.extraDrawDataGLBuffer.uploadData(batch.extraDrawInfo.data(), batch.extraDrawInfo.size() * sizeof(uint32_t), GLBuffer::Usage::OFTEN);
                    updatedObjectData = true;
                }

                for (auto& textureGroup : batch.objects) {

                    for (auto it = textureGroup.second.begin(); it != textureGroup.second.end();) {
                        if ( it->first->changedComponent<Component::Texture>()) {
                            it = removeObject2d((*it).first, textureGroup.first);

                            addObject((*it).first);
                            
                            it->first->getComponent<Component::Texture>()->frameCleanup();
                            continue;
                        }

                        // I lock it in the if statement intentionally so no deadlock 
                        if ( !it->first->changedComponent<Component::Transform2D>() || updatedObjectData) {
                            it++;
                            continue;
                        }

                        std::vector<float> newData = it->first->getDrawData();
                        memcpy((char*)batch.objectData.data() + (*it).second * batch.instanceDataSize, newData.data(), batch.instanceDataSize); 

                        objectDataUpdateRegions.push_back(
                            GLBufferUpdateRegion {
                                (uint8_t*)batch.objectData.data(),
                                batch.instanceDataSize,
                                (*it).second * batch.instanceDataSize
                            }
                        );

                        extraDataUpdateRegions.push_back(
                            GLBufferUpdateRegion {
                                (uint8_t*)batch.extraDrawInfo.data(),
                                8,
                                (*it).second * 8
                            }
                        );

                        it++;
                    }
                }
                batch.objectDataGLBuffer.partialUpdate(objectDataUpdateRegions);
                batch.extraDrawDataGLBuffer.partialUpdate(extraDataUpdateRegions);
            }
        }
    }



    void Renderer2D::initBatch(RenderBatch2D& batch) {
    }
    
    void Renderer2D::freeUnusedMemory() {
        for (auto& shader_group : draw_batches_2d) {
            for (RenderBatch2D& batch : shader_group.second) {
                cleanupBatch(batch);
            }
        }
    }
}