#pragma once

#include "scene/object/custom_render_object/custom_render_object.hpp"
#include "../text/text.hpp"
#include "scene/object/object.hpp"
#include "scene/object/2d/object2d.hpp"

namespace Sierra {
    class Button : public Object, public CustomRenderObject {
        public:
            Button(std::string label, Font& font);

        protected:
            void draw(Camera& camera) override;

        private:
            Text mText;
    };
}