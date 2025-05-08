#include "camera.hpp"

namespace GMTKEngine {
	Camera::Camera() {
        WARN("Nigga this shit ain't implemented");
	}

	Camera::~Camera() {
        WARN("Nigga this shit ain't implemented");
	}

    void Camera::update() {
		projection = glm::mat4(1.0f);

		projection = glm::translate(projection, transform.getPosition());

		projection = glm::rotate(projection, transform.getRotation().x, glm::vec3(1.0, 0.0, 0.0));
		projection = glm::rotate(projection, transform.getRotation().y, glm::vec3(0.0, 1.0, 0.0));
		projection = glm::rotate(projection, transform.getRotation().z, glm::vec3(0.0, 0.0, 1.0));
	}


	void Camera::apply_projection(GLuint location) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
	}

}