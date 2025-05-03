#include <iostream>

#include "window/window.hpp"
#include "manager/manager.hpp"
#include "renderer/renderer.hpp"


int main() {
    Window window = Window("le backroome", {1280, 720});

    Manager manager = Manager();

    Renderer rendeerer = Renderer(window);

}