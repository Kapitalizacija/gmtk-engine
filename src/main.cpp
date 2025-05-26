#include <iostream>
#include <stdfloat>
#include <chrono>
#include <array>

#include "window/window.hpp"
#include "scene/scene.hpp"
#include "scene/object/2d/ui/text/font/font.hpp"
#include "scene/object/2d/ui/text/text.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/shader/gl_shader.hpp"
#include "gl/util/vao/gl_vao.hpp"
#include "gl/texture/gl_texture.hpp"
#include "gl/util/gl_util.hpp"

#include "scene/object/components/transform/2d/transform2d.hpp"
#include "scene/object/components/physics/2d/physics2d.hpp"

#include "audio/al_device.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "audio/al_listener.hpp"

#include "scene/object/components/sound/sound.hpp"

#include "memory/memorypool.hpp"

#include <cmath>
#include <thread>

#define TEST_SOUND 1 //Set to 1 to play the example sound (will create the sound components no matter what)

using namespace GMTKEngine;
using namespace GMTKEngine::Component;

class TestObj : public Object2D {
    private:
        uint8_t buff[8192];
};

int main() {
    Window window = Window("GMTKEngine", {1280, 720});
    ALDevice audio;
    if (!audio.isValid()) {
        ERROR("OpenAL Device Error, exiting");
    }

    GLUtils::init();

    Scene scene = Scene();
    
    GLTexture tex = GLTexture("lepotec.jpg");
    GLTexture tex1 = GLTexture("bombardino.jpg");

    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    ResourceRef<Object2D> obj1 = scene.createObject<Object2D>();
    ResourceRef<Object2D> obj2 = scene.createObject<Object2D>();

    obj1->getComponent<Texture>()->setTexture(tex);
    obj2->getComponent<Texture>()->setTexture(tex1);

    obj1->setShader(shader);
    obj2->setShader(shader);

    obj1->getComponent<Transform2D>()->setPosition(glm::vec3(-100, 100, 0));
    obj2->getComponent<Transform2D>()->setPosition(glm::vec3(100, 100, 0));

    obj1->getComponent<Transform2D>()->setScale(glm::vec3(100, 100, 100));
    obj2->getComponent<Transform2D>()->setScale(glm::vec3(100, 100, 100));

    obj1->createComponent<Physics2D>();
    obj2->createComponent<Physics2D>();

    ResourceRef<Camera> cam = scene.getCamera();
    cam->setProjectionType(Camera::ProjectionType::ORTHOGRAPHIC);

    scene.getRenderer()->addObject2d(obj1);
    scene.getRenderer()->addObject2d(obj2);

    while ( !window.shouldClose() ) {
        auto res = obj1->getComponent<Physics2D>()->checkIntersection(obj2->getComponent<Physics2D>());
        DBG(res);

        obj1->getComponent<Transform2D>()->translate(glm::vec3(1.0f, 0.0f, 0.0f));

        window.update();
        scene.update();
    }

    GLUtils::cleanup();
}
