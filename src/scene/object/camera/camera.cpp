#include "camera.hpp"

namespace Sierra {
	Camera::Camera() {
		createComponent<Component::Transform2D>();
		createComponent<Component::Transform3D>();
		dimensions = glm::ivec2(1280, 720); //todo make not constant
	}

    void Camera::update(const StateInfo& stateInfo) {
		updateOrtho();
		updatePerspective();
	}

	void Camera::updateOrtho() {
		ResourceRef<Component::Transform2D> transform = getComponent<Component::Transform2D>();

		orthoProjection = glm::ortho(-(float)dimensions.x / 2, (float)dimensions.x / 2, -(float)dimensions.y / 2, (float)dimensions.y / 2); 
		orthoProjection = glm::translate(orthoProjection, glm::vec3(transform->getPosition()));
		orthoProjection = glm::rotate(orthoProjection, transform->getRotation(), glm::vec3(0, 0, 1));
	}

	void Camera::updatePerspective() {
		ResourceRef<Component::Transform3D> transform = getComponent<Component::Transform3D>();

		perspectiveProjection = glm::perspective(45.0f, 1.6667f, 0.0f, 1.0f);
		perspectiveProjection = glm::translate(perspectiveProjection, transform->getPosition());

		perspectiveProjection = glm::rotate(orthoProjection, transform->getRotation().x, glm::vec3(1.0, 0.0, 0.0));
		perspectiveProjection = glm::rotate(orthoProjection, transform->getRotation().y, glm::vec3(0.0, 1.0, 0.0));
		perspectiveProjection = glm::rotate(orthoProjection, transform->getRotation().z, glm::vec3(0.0, 0.0, 1.0));
	}

	void Camera::__applyProjection(GLuint location, ProjectionType type) {
		if (type == ProjectionType::ORTHOGRAPHIC)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(orthoProjection));
		else
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(perspectiveProjection));
	}
	
	void Camera::setDimensions(glm::ivec2 newDimensions) {
		dimensions = newDimensions;
	}

}