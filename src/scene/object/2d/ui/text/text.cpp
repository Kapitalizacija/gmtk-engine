#include "text.hpp"

namespace GMTKEngine {

    const uint32_t indices[] = { 
        0, 1, 2,
        1, 2, 3
    };

    const std::float32_t vertices[] = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f    
    };

    std::shared_ptr<GLShader> Text::shader = std::shared_ptr<GLShader>();
    std::shared_ptr<GLBuffer> Text::vertexGLBuff = std::shared_ptr<GLBuffer>();
    std::shared_ptr<GLBuffer> Text::indexGLBuff = std::shared_ptr<GLBuffer>();
    
    GLuint Text::posUniformLocation = 0;
    GLuint Text::scaleUniformLocation = 0;
    GLuint Text::rotationUniformLocation = 0;
    GLuint Text::textureUniformLocation = 0;
    GLuint Text::projectionUniformLocation = 0;
    
    const std::string Text::VERTEX_PATH = "test_shaders/text.vert";
    const std::string Text::FRAGMENT_PATH = "test_shaders/text.frag";

    Text::Text(Font& font, std::string text): text(text), font(font) {
        if (shader.get() == nullptr) {
            createGlobalResources();
        }        

        createComponent<Transform2D>();
        
        std::vector<float> offsets = font.getCharOffsets(text);
        createBuffers(offsets);
    }
    
    void Text::createBuffers(std::vector<float> offsetData) {
        offsetGLBuffer = GLBuffer(GLBuffer::Type::VERTEX, offsetData.data(), offsetData.size() * sizeof(float), GLBuffer::Usage::RARELY);

        GLAttribPointer ptr1{};
        ptr1.buff = &(*vertexGLBuff.get());
        ptr1.index = 0;
        ptr1.componentCount = 2;
        ptr1.type = GL_FLOAT;
        ptr1.stride = sizeof(float) * 2;
        ptr1.offset = 0;
        ptr1.isInstanced = false;


        GLAttribPointer ptr2{};
        ptr2.buff = &offsetGLBuffer;
        ptr2.index = 1;
        ptr2.componentCount = 2;
        ptr2.type = GL_FLOAT;
        ptr2.stride = sizeof(float) * 2;
        ptr2.offset = 0;
        ptr2.isInstanced = true;

        GLAttribPointer ptrs[] = {ptr1, ptr2};

        vao = GLVAO(ptrs, sizeof(ptrs) / sizeof(GLAttribPointer));
        vao.addEBO(*indexGLBuff);
    }

    void Text::createGlobalResources() {
        shader = std::make_shared<GLShader>("Text shader", VERTEX_PATH, FRAGMENT_PATH);
        vertexGLBuff = std::make_shared<GLBuffer>(GLBuffer::Type::VERTEX, vertices, sizeof(vertices), GLBuffer::Usage::RARELY);
        indexGLBuff = std::make_shared<GLBuffer>(GLBuffer::Type::INDEX, indices, sizeof(indices), GLBuffer::Usage::RARELY);

        posUniformLocation = glGetUniformLocation(shader->getProgram(), "pos");        
        rotationUniformLocation = glGetUniformLocation(shader->getProgram(), "rotation");        
        scaleUniformLocation = glGetUniformLocation(shader->getProgram(), "scale");        
        textureUniformLocation = glGetUniformLocation(shader->getProgram(), "tex");        
        projectionUniformLocation = glGetUniformLocation(shader->getProgram(), "projection");        
    }

    void Text::draw(Camera& camera) {
        std::shared_ptr<Transform2D> transform = getComponentLock<Transform2D>().value();

        glUseProgram(shader->getProgram());

        glBindVertexArray(vao.getVAO());

        glBindTexture(GL_TEXTURE_2D, font.getBitmap().getTexture());
        glActiveTexture(GL_TEXTURE0);

        glUniform3fv(posUniformLocation, 1, glm::value_ptr(transform->getPosition()));
        glUniform2fv(scaleUniformLocation, 1, glm::value_ptr(transform->getScale()));
        glUniform1f(rotationUniformLocation, transform->getRotation());

        camera.__applyProjection(projectionUniformLocation);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, text.length());

        glBindVertexArray(0);
    }

}