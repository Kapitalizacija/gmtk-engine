#include <iostream>
#include <stdfloat>
#include <chrono>
#include <array>
#include <numbers>

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
#include "scene/object/components/physics/2d/body2d.hpp"

#include "audio/al_device.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "audio/al_listener.hpp"

#include "scene/object/components/sound/sound.hpp"

#include "memory/mempool.hpp"

#include "input/input.hpp"

#include "engine/engine.hpp"

#include <cmath>
#include <thread>

#define TEST_SOUND 1 //Set to 1 to play the example sound (will create the sound components no matter what)

using namespace Sierra;
using namespace Sierra::Component;

class TestObj : public Object2D {
    private:
        uint8_t buff[8192];
};

int main() {
    std::string title = "Sierra";
    Engine engine = Engine(title, {1280, 720}, 1024);    

    /*
    Window window = Window("Sierra", {1280, 720});

    ALDevice audio;
    if (!audio.isValid()) {
        ERROR("OpenAL Device Error, exiting");
    }
    */
    //Scene scene = Scene(window);
    Scene scene = Scene(engine.getWindow()); //This is janky as fuck, holy shit someone take away my pc

    GLTexture tex = GLTexture("lepotec.jpg");
    GLTexture tex1 = GLTexture("bombardino.jpg");

    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    PhysicsConstants constants{};
    constants.airDrag = 0.01;
    constants.g = glm::vec3(0.0f, -300.0f, 0.0f);
    constants.top_down_physics = false;

    ResourceRef<PhysicsManager2D> physicsManager2D = scene.getPhysicsManager2D();
    physicsManager2D->setConstants(constants);


    ResourceRef<Object2D> obj1 = scene.createObject<Object2D>();
    ResourceRef<Object2D> obj2 = scene.createObject<Object2D>();

    obj1->getComponent<Texture>()->setTexture(tex);
    obj2->getComponent<Texture>()->setTexture(tex1);

    obj1->setShader(shader);
    obj2->setShader(shader);

    obj1->getComponent<Transform2D>()->setPosition(glm::vec3(300, 0, 0));
    obj2->getComponent<Transform2D>()->setPosition(glm::vec3(0, 0, 0));

    obj1->getComponent<Transform2D>()->setScale(glm::vec3(100));
    obj2->getComponent<Transform2D>()->setScale(glm::vec3(100));

    obj1->createComponent<Body2D>();
    obj2->createComponent<Body2D>();

    physicsManager2D->addBody(obj1->getComponent<Body2D>());
    physicsManager2D->addBody(obj2->getComponent<Body2D>());

    obj1->getComponent<Body2D>()->setIsSimulated(true);
    obj2->getComponent<Body2D>()->setIsSimulated(true);
    obj1->getComponent<Body2D>()->setIsAffectedByGravity(false);
    obj2->getComponent<Body2D>()->setIsAffectedByGravity(false);

    scene.getRenderer()->addObject2d(obj1);
    scene.getRenderer()->addObject2d(obj2);


    Body2D::Info info{0};
    info.elasticity = 0.5;
    info.friction = 0.1f;
    info.mass = 100.0f;

    ResourceRef<Camera> cam = scene.getCamera();

    //std::shared_ptr<Scene> s(&scene);

    (void)engine.loadScene(&scene); //surely

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    double fps = 0.0;

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    //obj2->getComponent<Body2D>()->applyImpulse(glm::vec3(-10000, 0, 0));

    while (!engine.getWindow()->shouldClose()) {
        //Some basic fps counter login here, should be moved sometime

        fps = 1.0 / scene.getDeltaTime();

        std::string title = "FPS: " + std::to_string(static_cast<int>(fps));
        glfwSetWindowTitle(engine.getWindow()->get_glfw_window(), title.c_str());

        //obj1->getComponent<Transform2D>()->translate(glm::vec3(5.0f, 0.0f, 0.0f));
        //obj1->getComponent<Transform2D>()->setRotation(glfwGetTime() * 10);

        //glm::ivec2 pos = Input::Input::getMousePosition();
        //DBG("Mouse pos; X: " << pos.x << " Y: " << pos.y);
        //
        
        float moveSpeed = 6.f;
        glm::vec3 curPos = obj1->getComponent<Transform2D>()->getPosition();
        if (InputHandler::isKeyPressed(GLFW_KEY_W))
            curPos.y += moveSpeed;
        if (InputHandler::isKeyPressed(GLFW_KEY_S))
            curPos.y -= moveSpeed;
        if (InputHandler::isKeyPressed(GLFW_KEY_D))
            curPos.x += moveSpeed;
        if (InputHandler::isKeyPressed(GLFW_KEY_A))
            curPos.x -= moveSpeed;
        obj1->getComponent<Transform2D>()->setPosition(curPos);
        engine.update();
    }

    //engine.unloadScene();
    Debug::Renderer::cleanup();
    GLUtils::cleanup();
}
