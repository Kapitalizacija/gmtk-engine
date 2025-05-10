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
    ALDevice audio;
    if (!audio.isValid()) {
        ERROR("OpenAL Device Error, exiting");
    }

    Scene scene = Scene();
    
    std::float32_t verts[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };
    
    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    
    
    GLTexture tex = GLTexture("image.png");
    GLTexture tex1 = GLTexture("bombardino.jpg");
    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    GLBuffer buff = GLBuffer(GLBuffer::Type::VERTEX, (uint8_t*)verts, sizeof(verts), GLBuffer::Usage::RARELY);
    GLBuffer index_buff = GLBuffer(GLBuffer::Type::INDEX, (uint8_t*)indices, sizeof(indices), GLBuffer::Usage::RARELY);

    GLAttribPointer attributes[] = {
        {&buff, 0, 2, GL_FLOAT, 8, 0}
    };

    GLVAO vao = GLVAO(attributes, 1);

    //TODO: Test the Sound component abstraction here or something
    /*ALBuffer buffer;
    if (!buffer.loadFromFile("file_example_WAV_10MG.wav")) {
        ERROR("Failed to load audio");
        return 1;
    }

    ALSound sound(buffer);
    sound.setLooping(false);
    sound.setGain(1.0f);
    sound.play();*/

    //The update() method needs to be impemented fully before this will work :(
    Object2D *obj;
    scene.create_object(&obj);
    obj->setShader(shader);
    ((Texture*)obj->getComponent<Texture>())->setTexture(tex);
    scene.addToRenderer(obj);
    

    Object2D* obj1;
    scene.create_object(&obj1);
    obj1->setShader(shader);
    (obj1->getComponent<Texture>())->setTexture(tex1);
    obj1->getComponent<Transform2D>()->setPosition(glm::vec2(-0.5, -0.5));
    scene.addToRenderer(obj1);

    while ( !window.should_close() ) {
        obj1->getComponent<Transform2D>()->setPosition(glm::vec2(glfwGetTime() / 10 - 1, -0.5));
        obj1->getComponent<Transform2D>()->setRotation(glfwGetTime());
        scene.update();
        window.update();
    }

}
