#include <iostream>

#include "window/window.hpp"
#include "manager/manager.hpp"
#include "renderer/renderer.hpp"
#include "renderer/util/buffer/gl_buffer.hpp"

#include "manager/object/components/TestComponent.hpp"

using namespace GMTKEngine;

int main() {
    Window window = Window("le backroome", {1280, 720});

    Manager manager = Manager();
    
    //Create a test component and add it to a test object.
    TestComponent* testComp = new TestComponent();
    Object* obj = new Object();
    obj->create_component(&testComp);
    manager.create_object(&obj);

    Renderer renderer = Renderer(window);

    char abc[] = {1, 2, 3};
    GLBuffer buff = GLBuffer(abc, sizeof abc, GLBuffer::VERTEX, GLBuffer::OFTEN);

}