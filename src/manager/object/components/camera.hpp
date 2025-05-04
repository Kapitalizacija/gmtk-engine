#pragma once

#include <glm/glm.hpp>

#include "manager/object/components/transform.hpp"
#include "manager/object/object.hpp"

namespace GMTKEngine {
	class Camera : Object {
		public:
			Camera();
			~Camera();

		protected:

            virtual void start() { return; }
            virtual void early_update() { return; }
			virtual void update() override;
            virtual void late_update() { return; }

		private:
			Transform transform;

			std::string mComponentName;
	};
}