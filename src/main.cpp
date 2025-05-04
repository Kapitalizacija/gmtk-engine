#include <iostream>

#include "window/window.hpp"
#include "manager/manager.hpp"
#include "renderer/renderer.hpp"
#include "renderer/util/buffer/gl_buffer.hpp"
#include "renderer/util/shader/gl_shader.hpp"

#include "manager/object/components/TestComponent.hpp"

using namespace GMTKEngine;

int main() {
    Window window = Window("le backroome", {1280, 720});

    Manager manager = Manager();
    
    //Create a test component and add it to a test object.
    TestComponent* testComp;
    Object* obj;
    manager.create_object(&obj);
    obj->create_component(&testComp);

    Renderer renderer = Renderer(window);

    char abc[] = {1, 2, 3};
    GLBuffer buff = GLBuffer(abc, sizeof abc, GLBuffer::VERTEX, GLBuffer::OFTEN);
    GLShader shader = GLShader("test_shader", "test_shaders/tri.vert", "test_shaders/tri.frag");

    while ( !window.should_close() ) {
        window.update();
    }

}