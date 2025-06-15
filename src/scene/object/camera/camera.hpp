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

			void setDimensions(glm::ivec2 newDimensions);
			
			void __applyProjection(GLuint location, ProjectionType type);

			protected:
			
            virtual void start() { return; }
            virtual void earlyUpdate(const StateInfo& stateInfo) { return; }
			virtual void update(const StateInfo& stateInfo) override;
            virtual void lateUpdate(const StateInfo& stateInfo) { return; }

		private:
			void updateOrtho();
			void updatePerspective();

			glm::mat4 orthoProjection;
			glm::mat4 perspectiveProjection;

			glm::ivec2 dimensions;
	};
}