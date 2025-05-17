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

    std::shared_ptr<GLShader> Text::SHADER = std::shared_ptr<GLShader>();
    std::shared_ptr<GLBuffer> Text::VERTEXGLBUFF = std::shared_ptr<GLBuffer>();
    std::shared_ptr<GLBuffer> Text::INDEXGLBUFF = std::shared_ptr<GLBuffer>();
    
    GLuint Text::posUniformLocation = 0;
    GLuint Text::scaleUniformLocation = 0;
    GLuint Text::rotationUniformLocation = 0;
    GLuint Text::textureUniformLocation = 0;
    GLuint Text::projectionUniformLocation = 0;

    uint32_t Text::USE_COUNT = 0;

    
    const std::string Text::VERTEX_PATH = "test_shaders/text.vert";
    const std::string Text::FRAGMENT_PATH = "test_shaders/text.frag";

    Text::Text(Font& font, std::string text): text(text), font(font) {
        if (SHADER.get() == nullptr) {
            createGlobalResources();
        }        

        createComponent<Transform2D>();
        
        std::vector<float> offsets = font.getCharOffsets(text);
        createBuffers(offsets);

        USE_COUNT++;
    }
    
    void Text::createBuffers(std::vector<float> offsetData) {
        offsetGLBuffer = GLBuffer(GLBuffer::Type::STORAGE, offsetData.data(), offsetData.size() * sizeof(float), GLBuffer::Usage::RARELY);

        GLAttribPointer ptr1{};
        ptr1.buff = &(*VERTEXGLBUFF.get());
        ptr1.index = 0;
        ptr1.componentCount = 2;
        ptr1.type = GL_FLOAT;
        ptr1.stride = sizeof(float) * 2;
        ptr1.offset = 0;
        ptr1.isInstanced = false;

        GLAttribPointer ptrs[] = {ptr1};

        vao = GLVAO(ptrs, sizeof(ptrs) / sizeof(GLAttribPointer));
        vao.addEBO(*INDEXGLBUFF);
    }

    void Text::createGlobalResources() {
        SHADER = std::make_shared<GLShader>("Text shader", VERTEX_PATH, FRAGMENT_PATH);
        VERTEXGLBUFF = std::make_shared<GLBuffer>(GLBuffer::Type::VERTEX, vertices, sizeof(vertices), GLBuffer::Usage::RARELY);
        INDEXGLBUFF = std::make_shared<GLBuffer>(GLBuffer::Type::INDEX, indices, sizeof(indices), GLBuffer::Usage::RARELY);

        posUniformLocation = glGetUniformLocation(SHADER->getProgram(), "pos");        
        rotationUniformLocation = glGetUniformLocation(SHADER->getProgram(), "rotation");        
        scaleUniformLocation = glGetUniformLocation(SHADER->getProgram(), "scale");        
        textureUniformLocation = glGetUniformLocation(SHADER->getProgram(), "tex");        
        projectionUniformLocation = glGetUniformLocation(SHADER->getProgram(), "projection");        
    }
    

    void Text::draw(Camera& camera) {
        ComponentRef<Transform2D> transform = getComponent<Transform2D>();

        glUseProgram(SHADER->getProgram());

        glBindVertexArray(vao.getVAO());

        offsetGLBuffer.bindBase(1);

        glBindTexture(GL_TEXTURE_2D, font.getBitmap().getTexture());
        glActiveTexture(GL_TEXTURE0);

        glUniform3fv(posUniformLocation, 1, glm::value_ptr(transform->getPosition()));
        glUniform2fv(scaleUniformLocation, 1, glm::value_ptr(transform->getScale()));
        glUniform1f(rotationUniformLocation, transform->getRotation());

        camera.__applyProjection(projectionUniformLocation);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, text.length());

        glBindVertexArray(0);
    }

    Text::~Text() {
        USE_COUNT--;

        if (USE_COUNT == 0) {
            SHADER.reset();
            VERTEXGLBUFF.reset();
            INDEXGLBUFF.reset();
        }
    }
}