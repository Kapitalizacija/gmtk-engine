#include "gl_util.hpp"

const uint32_t indices[] = { // the most barebones indices needed for a square, therefore I feel comfortable making it a global cosntant
    0, 1, 2,
    1, 2, 3
};

/*const std::float32_t vertices[] = {
     0.0f,  0.0f,
     0.0f,  1.0f,
     1.0f,  0.0f,
     1.0f,  1.0f
};*/

const std::float32_t vertices[] = {
    -0.5f, -0.5f,
    -0.5f,  0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f    
};


namespace Sierra {
    std::unique_ptr<GLBuffer> GLUtils::squareVertexBuffer = {};
    std::unique_ptr<GLBuffer> GLUtils::squareIndexBuffer = {};

    void GLUtils::init() {
        loadBuffers();
    }

    void GLUtils::loadBuffers() {
        squareVertexBuffer = std::make_unique<GLBuffer>(GLBuffer::Type::VERTEX, (uint8_t*)vertices, sizeof(vertices), GLBuffer::Usage::RARELY);
        squareIndexBuffer = std::make_unique<GLBuffer>(GLBuffer::Type::INDEX, (uint8_t*)indices, sizeof(indices), GLBuffer::Usage::RARELY);
    }

    GLBuffer& GLUtils::getSquareVertexBuffer() {
        return *squareVertexBuffer;
    }

    GLBuffer& GLUtils::getSquareIndexBuffer() {
        return *squareIndexBuffer;
    }

    void GLUtils::cleanup() {
        squareIndexBuffer.~unique_ptr();
        squareVertexBuffer.~unique_ptr();
    }
}