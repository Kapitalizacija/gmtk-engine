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
    //Allocate a small mempool for the lulz
    Memory::Mempool mempool = Memory::Mempool(1048576); //1 MiB because I feel special

    Window window = Window("Sierra", {1280, 720});

    Input::Input::init(&window);

    ALDevice audio;
    if (!audio.isValid()) {
        ERROR("OpenAL Device Error, exiting");
    }


    Scene scene = Scene();
    
    GLTexture tex = GLTexture("lepotec.jpg");
    GLTexture tex1 = GLTexture("bombardino.jpg");

    GLShader shader = GLShader("test_shader", "test_shaders/sprite_2d.vert", "test_shaders/sprite_2d.frag"); // fellas in paris

    Font font = Font("fonts/arial-font/arial.ttf");

    ResourceRef<Text> text = scene.createObject<Text>(font, "sigma sigma (WORKING TEXT??");
    text->getComponent<Transform2D>()->setPosition(glm::vec3(-500, 0, 0));
    text->getComponent<Transform2D>()->setScale(glm::vec3(75.0));
    scene.getRenderer()->addCustomRenderObject(text);

    PhysicsConstants constants{};
    constants.airDrag = 0.5f;
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

    obj1->getComponent<Transform2D>()->setPosition(glm::vec3(-100, 0, 0));
    obj2->getComponent<Transform2D>()->setPosition(glm::vec3(200, 0, 0));

    obj1->getComponent<Transform2D>()->setScale(glm::vec3(50));
    obj2->getComponent<Transform2D>()->setScale(glm::vec3(50));

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
    info.elasticity = 0.2f;
    info.friction = 0.1f;
    info.mass = 100.0f;


    ResourceRef<Camera> cam = scene.getCamera();
    cam->setProjectionType(Camera::ProjectionType::ORTHOGRAPHIC);


    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    double fps = 0.0;

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    obj1->getComponent<Body2D>()->applyImpulse(glm::vec3(100, 0, 0));
    //obj2->getComponent<Body2D>()->applyImpulse(glm::vec3(-10000, 0, 0));

    while (!window.shouldClose()) {
        //Some basic fps counter login here, should be moved sometime

        fps = 1.0 / scene.getDeltaTime();

        std::string title = "FPS: " + std::to_string(static_cast<int>(fps));
        glfwSetWindowTitle(window.get_glfw_window(), title.c_str());

        //obj1->getComponent<Transform2D>()->translate(glm::vec3(5.0f, 0.0f, 0.0f));
        //obj1->getComponent<Transform2D>()->setRotation(glfwGetTime() * 10);

        //glm::ivec2 pos = Input::Input::getMousePosition();
        //DBG("Mouse pos; X: " << pos.x << " Y: " << pos.y);
        //
        //if (Input::Input::isKeyPressed(GLFW_KEY_W))
        //    DBG("Pressing W!");
        
        window.update();
        scene.update();

        Input::Input::endFrame();
    }

    Debug::Renderer::cleanup();
    GLUtils::cleanup();


}
