#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "scene/object/components/transform/3d/transform3d.hpp"
#include "scene/object/components/transform/2d/transform2d.hpp"
#include "scene/object/object.hpp"

namespace Sierra {
	class Camera : public Object {
		friend class Renderer2D;
		friend class Scene;

		public:
			enum class ProjectionType {
				ORTHOGRAPHIC,
				PERPSECTIVE
			};

			Camera();

			void setProjectionType(ProjectionType newType);
			void setDimensions(glm::ivec2 newDimensions);
			
			void __applyProjection(GLuint location);

			protected:
			
            virtual void start() { return; }
            virtual void earlyUpdate() { return; }
			virtual void update() override;
            virtual void lateUpdate() { return; }

		private:
			void updateOrtho();
			void updatePerspective();

			ProjectionType projectionType;

			glm::mat4 projection;
			glm::ivec2 dimensions;
	};
}