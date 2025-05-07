#include <iostream>
#include <stdfloat>
#include <chrono>

#include "window/window.hpp"
#include "scene/scene.hpp"
#include "gl/util/buffer/gl_buffer.hpp"
#include "gl/util/shader/gl_shader.hpp"
#include "gl/util/vao/gl_vao.hpp"
#include "gl/texture/gl_texture.hpp"

#include "scene/object/components/transform/transform.hpp"

#include "audio/al_device.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "audio/al_listener.hpp"

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
        return 1;
    }

    Scene manager = Scene();
    GLTexture tex = GLTexture("image.png");

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

    
    GLShader shader = GLShader("test_shader", "test_shaders/tri.vert", "test_shaders/tri.frag"); // fellas in paris



    GLBuffer buff = GLBuffer((uint8_t*)verts, sizeof(verts), GLBuffer::VERTEX, GLBuffer::RARELY);
    GLBuffer index_buff = GLBuffer((uint8_t*)indices, sizeof(indices), GLBuffer::INDEX, GLBuffer::RARELY);

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

    while ( !window.should_close() ) {
        glBindVertexArray(vao.get_vao());
        glUseProgram(shader.get_program());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff.get_buffer());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        window.update();
    }

}
