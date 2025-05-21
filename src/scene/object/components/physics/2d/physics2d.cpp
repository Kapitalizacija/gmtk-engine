#include "physics2d.hpp"

//The physics 2D engine hooks directly into the Transform2D component
namespace GMTKEngine {
    namespace Component {
        Physics2D::Physics2D(Shape shape): shape(shape) {
            mForceDirection = glm::vec3(0.f, 0.f, 0.f);
        }
    }
}