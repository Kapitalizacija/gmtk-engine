#include <iostream>
#include <stdfloat>
#include <chrono>

#include "window/window.hpp"
#include "scene/scene.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/shader/gl_shader.hpp"
#include "gl/util/vao/gl_vao.hpp"
#include "gl/texture/gl_texture.hpp"

#include "scene/object/components/transform/2d/transform2d.hpp"

#include "audio/al_device.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "audio/al_listener.hpp"

#include "scene/object/components/sound/sound.hpp"

#include "memory/memorypool.hpp"

#include <cmath>
#include <thread>

using namespace GMTKEngine;

class TestObj : public Object2D {
    private:
        uint8_t buff[8192];
};

int main() {
    Window window = Window("GMTKEngine", {1280, 720});
    //ALDevice audio;
    //if (!audio.isValid()) {
    //    ERROR("OpenAL Device Error, exiting");
    //}

    Scene scene = Scene();
    
    GLTexture tex = GLTexture("image.png");
    GLTexture tex1 = GLTexture("bombardino.jpg");
    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    //TODO: Test the Sound component abstraction here or something
    /*
    ALBuffer buffer;
    if (!buffer.loadFromFile("example.mp3")) {
        ERROR("Failed to load audio");
        return 1;
    }

    ALSound sound(buffer);
    sound.setLooping(false);
    sound.setGain(1.0f);
    sound.play();*/

    //The update() method needs to be impemented fully before this will work :(
    Object2D *obj = scene.create_object<Object2D>();
    obj->setShader(shader);
    ((Texture*)obj->getComponent<Texture>())->setTexture(tex1);
    obj->getComponent<Transform2D>()->setPosition(glm::vec2(500, 500));
    obj->getComponent<Transform2D>()->setScale(glm::vec2(400, 400));
    obj->getComponent<Transform2D>()->setDepth(0.0);
    scene.addToRenderer(obj);
    

    Object2D* obj1 = scene.create_object<Object2D>();
    obj1->setShader(shader);
    (obj1->getComponent<Texture>())->setTexture(tex);
    obj1->getComponent<Transform2D>()->setPosition(glm::vec2(500, 500));
    obj1->getComponent<Transform2D>()->setScale(glm::vec2(400, 400));
    obj1->getComponent<Transform2D>()->setDepth(1.0);
    scene.addToRenderer(obj1);

    Camera* cam = scene.getCamera();
    cam->setProjectionType(Camera::ProjectionType::ORTHOGRAPHIC);

    while ( !window.should_close() ) {
        obj1->getComponent<Transform2D>()->setPosition(glm::vec2(sin(glfwGetTime() )* 1000, 400) );
        obj1->getComponent<Transform2D>()->setRotation(glfwGetTime());

        if (glfwGetTime() > 1) {
            obj1->getComponent<Texture>()->setTexture(tex1);
        }

        scene.update();
        window.update();
    }

}
