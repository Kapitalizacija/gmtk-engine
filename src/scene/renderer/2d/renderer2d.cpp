#include "renderer2d.hpp"

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

    void Renderer2D::render() {
        GLint texture_indices[32];
        for (size_t i = 0; i < 32; i++) {
            texture_indices[i] = i;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (auto& shaderGroup : draw_batches_2d) {
            glUseProgram(shaderGroup.first);

            GLint texturesUniformLoc = glGetUniformLocation(shaderGroup.first, "textures");
            GLint textureOffsetsLoc = glGetUniformLocation(shaderGroup.first, "offsets");

            glUniform1iv(texturesUniformLoc, 32, texture_indices);

            for (RenderBatch2D& batch : shaderGroup.second) {
                std::vector<GLint> textureBatchOffsets;
                textureBatchOffsets.resize(32);

                batch.objectDataGLBuffer.upload_data(batch.objectData.data(), batch.objectData.size() * sizeof(std::float32_t), GLBuffer::Usage::OFTEN);
                
                auto it = batch.objects.begin();
                for ( size_t i = 0; i < batch.objects.size(); i++) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, it->first);
                    textureBatchOffsets[i] = it->second.size();
                    it++;
                }

                glUniform1iv(textureOffsetsLoc, 32, textureBatchOffsets.data());

                glBindVertexArray(batch.vao.getVAO());


                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, batch.instanceCount);
            }
        }

        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Renderer2D::addObject2d(Object2D* object) {
        auto& shader_group = draw_batches_2d[object->program];
        
        GLuint textureID = (object->getComponent<Texture>())->getRawHandle();

        bool found_group = false;
        for (auto& tex_group : shader_group) {
            if (tex_group.objects.find(textureID) == tex_group.objects.end() && tex_group.objects.size() >= 32) {
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

        GLuint textureID = (object->getComponent<Texture>())->getRawHandle();

        auto& shader_group = draw_batches_2d[object->program];
        for (auto& batch : shader_group) {

            if (batch.objects.find(textureID) != batch.objects.end()) {
         

                batch.clearQueue.push_back(batch.objects[textureID][object]);
                batch.objects[textureID].erase(object);

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

        GLuint textureID = (object->getComponent<Texture>())->getRawHandle();

        batch.objects[textureID][object] = batch.instanceCount;
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

        size_t dstStartOffset = batch.clearQueue[0] * batch.instanceDataSize + batch.instanceDataSize;
        for ( size_t i = 1; i < batch.clearQueue.size() - 1; i += 2) {
            size_t srcStartOffset = batch.clearQueue[i] * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.clearQueue[i+1] * batch.instanceDataSize;

            dstStartOffset += (srcEndOffset - srcStartOffset);

            if (srcEndOffset - srcStartOffset == 0) {
                continue;
            }

            memcpy(batch.objectData.data() + dstStartOffset, batch.objectData.data() + srcStartOffset, srcEndOffset - srcStartOffset);

        }

        if ( batch.clearQueue.size() & 1 == 1 ){
            size_t srcStartOffset = batch.clearQueue.back() * batch.instanceDataSize + batch.instanceDataSize;
            size_t srcEndOffset = batch.objectData.size() * sizeof(std::float32_t);

            if (srcEndOffset - srcStartOffset == 0) {
               return; 
            }

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
        batch.objectDataGLBuffer = GLBuffer(GLBuffer::Type::VERTEX);

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
        ptr1.componentCount = 3;
        ptr1.type = GL_FLOAT;
        ptr1.stride = batch.instanceDataSize;
        ptr1.offset = 0;
        ptr1.isInstanced = true;

        GLAttribPointer ptr2;
        ptr2.buff = &batch.objectDataGLBuffer;
        ptr2.index = 2;
        ptr2.componentCount = 4;
        ptr2.type = GL_FLOAT;
        ptr2.stride = batch.instanceDataSize;
        ptr2.offset = 12;
        ptr2.isInstanced = true;

        GLAttribPointer ptr3;
        ptr3.buff = &batch.objectDataGLBuffer;
        ptr3.index = 3;
        ptr3.componentCount = 3;
        ptr3.type = GL_FLOAT;
        ptr3.stride = batch.instanceDataSize;
        ptr3.offset = 28;
        ptr3.isInstanced = true;

        GLAttribPointer ptrs[] = {ptr0, ptr1, ptr2, ptr3};

        batch.vao = GLVAO(ptrs, 4);
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