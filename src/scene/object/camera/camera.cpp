#include "camera.hpp"

namespace GMTKEngine {
	Camera::Camera() {
		createComponent<Transform2D>();
		createComponent<Transform3D>();
		projectionType = ProjectionType::ORTHOGRAPHIC;
		dimensions = glm::ivec2(1280, 720);
	}

    void Camera::update() {
		if (projectionType == ProjectionType::ORTHOGRAPHIC) {
			updateOrtho();
		} else {
			updatePerspective();
		}
	}

	void Camera::updateOrtho() {
		std::shared_ptr<Transform2D> transform = getComponentLock<Transform2D>().value();

		projection = glm::ortho(-(float)dimensions.x / 2, (float)dimensions.x / 2, -(float)dimensions.y / 2, (float)dimensions.y / 2); 
		projection = glm::translate(projection, glm::vec3(transform->getPosition(), 0.0));
		projection = glm::rotate(projection, transform->getRotation(), glm::vec3(0, 0, 1));
	}

	void Camera::updatePerspective() {
		std::shared_ptr<Transform3D> transform = getComponentLock<Transform3D>().value();

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
	
	void Camera::setDimensions(glm::ivec2 newDimensions) {
		dimensions = newDimensions;
	}

}