#include <iostream>
#include <stdfloat>

#include "window/window.hpp"
#include "manager/manager.hpp"
#include "renderer/util/buffer/gl_buffer.hpp"
#include "renderer/util/shader/gl_shader.hpp"
#include "renderer/util/vao/gl_vao.hpp"

#include "manager/object/components/transform.hpp"

using namespace GMTKEngine;

int main() {
    Window window = Window("le backroome", {1280, 720});

    Manager manager = Manager();
    
    //Create a test component and add it to a test object.
    Transform* transform;
    Object* obj;
    manager.create_object(&obj);
    obj->setName("Object1");
    obj->create_component(&transform);

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


    GLBuffer buff = GLBuffer((uint8_t*)verts, sizeof(verts), GLBuffer::VERTEX, GLBuffer::RARELY);
    GLBuffer index_buff = GLBuffer((uint8_t*)indices, sizeof(indices), GLBuffer::INDEX, GLBuffer::RARELY);

    GLAttribPointer attributes[] = {
        {&buff, 0, 2, GL_FLOAT, 8, 0}
    };

    GLShader shader = GLShader("test_shader", "test_shaders/tri.vert", "test_shaders/tri.frag"); // niggas in paris
    GLVAO vao = GLVAO(attributes, 1);


    while ( !window.should_close() ) {
        glBindVertexArray(vao.get_vao());
        glUseProgram(shader.get_program());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff.get_buffer());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        window.update();
    }

}