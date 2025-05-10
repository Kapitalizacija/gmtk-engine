#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "scene/object/components/transform/3d/transform3d.hpp"
#include "scene/object/object.hpp"

namespace GMTKEngine {
	class Camera : Object {
		public:
			Camera();
			~Camera();

			void apply_projection(GLuint location);

		protected:

            virtual void start() { return; }
            virtual void earlyUpdate() { return; }
			virtual void update() override;
            virtual void lateUpdate() { return; }

		private:
			Transform transform;
			glm::mat4 projection;
	};
}