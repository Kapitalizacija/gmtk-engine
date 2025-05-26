#pragma once

#include <stdfloat>
#include <memory>

#include "buffer/gl_buffer.hpp"

namespace Sierra {
    class GLUtils {
        public: // TODO make protected when engine class is made
            static void init();
            static void cleanup();


            static GLBuffer& getSquareVertexBuffer();
            static GLBuffer& getSquareIndexBuffer();

        private:
            static void loadBuffers();

            static std::unique_ptr<GLBuffer> squareVertexBuffer;
            static std::unique_ptr<GLBuffer> squareIndexBuffer;
    };
}