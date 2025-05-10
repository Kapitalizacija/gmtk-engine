#include "camera.hpp"

namespace GMTKEngine {
	Camera::Camera() {
		createComponent<Transform2D>();
		createComponent<Transform3D>();
		projectionType = ProjectionType::ORTHOGRAPHIC;
	}

    void Camera::update() {
		if (projectionType == ProjectionType::ORTHOGRAPHIC) {
			updateOrtho();
		} else {
			updatePerspective();
		}
	}

	void Camera::updateOrtho() {
		Transform2D *transform = getComponent<Transform2D>();

		projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f); // TODO make these not hardcoded
//		projection = glm::translate(projection, glm::vec3(transform->getPosition(), 0.0));
//		projection = glm::rotate(projection, transform->getRotation(), glm::vec3(0, 0, 1));
	}

	void Camera::updatePerspective() {
		Transform3D *transform = getComponent<Transform3D>();

		projection = glm::perspective(45.0f, 1.6667f, 0.0f, 1.0f);
		projection = glm::translate(projection, transform->getPosition());

		projection = glm::rotate(projection, transform->getRotation().x, glm::vec3(1.0, 0.0, 0.0));
		projection = glm::rotate(projection, transform->getRotation().y, glm::vec3(0.0, 1.0, 0.0));
		projection = glm::rotate(projection, transform->getRotation().z, glm::vec3(0.0, 0.0, 1.0));
	}

	void Camera::applyProjection(GLuint location) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
	}
	
	void Camera::setProjectionType(ProjectionType newType) {
		projectionType = newType;
	}

}