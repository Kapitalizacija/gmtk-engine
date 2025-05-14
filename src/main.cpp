#include <iostream>
#include <stdfloat>
#include <chrono>
#include <array>

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
    

    Camera* cam = scene.getCamera();
    cam->setProjectionType(Camera::ProjectionType::ORTHOGRAPHIC);

    std::array<std::weak_ptr<Object2D>, 128*72> objs;

    for(int y = 0; y < 72; y++) {
        for (int x = 0; x < 128; x++) {
            std::weak_ptr<Object2D> obj;

            obj = scene.createObject<Object2D>();
            auto obj_shared = obj.lock();
            obj_shared->setShader(shader);
            obj_shared->getComponentLock<Texture>().value()->setTexture(tex);
            obj_shared->getComponentLock<Transform2D>().value()->setPosition(glm::vec2((float)x * 10 - 640, (float)y * 10 - 360));
            obj_shared->getComponentLock<Transform2D>().value()->setScale(glm::vec2(10.0f, 10.0f));
            scene.addToRenderer(obj);

            objs[y * 128 + x] = obj;
        }
    }
    
    int i = 0;
    while ( !window.shouldClose() ) {
        scene.update();
        window.update();

        if (i < 128 * 72) {
            scene.removeFromRenderer(objs[i]);
        }
        i++;
    }

}
