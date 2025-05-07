#include "object2D.hpp"

namespace GMTKEngine {

    void Object2D::setShader(GLShader& shader) {
        if (rendered) {
            ERROR("Tried to change an object's shader while it's rendering is enabled");
            return;
        }

        program = shader.get_program();
    }

    std::vector<float> Object2D::getDrawData() {
        return {
            mPosition.x, mPosition.y, mPosition.z,
            mRotation.x, mRotation.y, mRotation.z, mRotation.w,
            mScale.x,    mScale.y,    mScale.z  
        };
    }
}