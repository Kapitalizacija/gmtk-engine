#include "object2d.hpp"

namespace GMTKEngine {
    std::vector<float> Object2D::getDrawData() {
        return {
            mPosition.x, mPosition.y, mPosition.z,
            mRotation.x, mRotation.y, mRotation.z, mRotation.w,
            mScale.x,    mScale.y,    mScale.z  
        };
    }
}