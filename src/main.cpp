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

#include "scene/object/components/transform/2d/transform2d.hpp"

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

    Scene scene = Scene();
    
    GLTexture tex = GLTexture("image.png");
    GLTexture tex1 = GLTexture("bombardino.jpg");
    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    Font font = Font("fonts/font.ttf");
    auto text = scene.createObject<Text>(font, "no homo");

    ResourceRef<Transform2D> transform = text->getComponent<Transform2D>();
    transform->setScale(glm::vec3(300, 300, 300));
    transform->translate(glm::vec3(0.0, 0.0, 0.1));
    
    scene.getRenderer()->addCustomRenderObject(text);
    


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
    

    ResourceRef<Camera> cam = scene.getCamera();
    cam->setProjectionType(Camera::ProjectionType::ORTHOGRAPHIC);

    std::array<ResourceRef<Object2D>, 128*72> objs;

    for(int y = 0; y < 72; y++) {
        for (int x = 0; x < 128; x++) {
            ResourceRef<Object2D> obj;

            obj = scene.createObject<Object2D>();
            obj->setShader(shader);
            obj->getComponent<Texture>()->setTexture(tex);
            obj->getComponent<Transform2D>()->setPosition(glm::vec3((float)x * 10 - 640, (float)y * 10 - 360, 0.0));
            obj->getComponent<Transform2D>()->setScale(glm::vec3(10.0f, 10.0f, 0.0));
            scene.getRenderer()->addObject2d(obj);

            objs[y * 128 + x] = obj;
        }
    }

    ResourceRef<Object2D> obj = scene.createObject<Object2D>();
    obj->setShader(shader);
    //obj_shared->getComponentLock<Transform2D>().value()->setScale(glm::vec3(1280, 720, 0));
    //obj_shared->getComponentLock<Transform2D>().value()->setPosition(glm::vec3(-640, -360, 0));

    obj->createComponent<Sound>();
//    auto soundShared = obj_shared->getComponent<Sound>().value();
//    bool res = soundShared->loadSound("example", "example.mp3");
//    DBG("Sound load status: " << res);
//    if (TEST_SOUND) {
//        soundShared->setIsLooping(false);
//        soundShared->setGain(1.f);
//        soundShared->setPosition(glm::vec3(100.f, 0.f, 0.f));
//        soundShared->playSound("example");
//    }
    
    while ( !window.shouldClose() ) {
        scene.update();
        window.update();
    }

}
