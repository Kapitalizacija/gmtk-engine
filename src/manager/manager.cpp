#include "manager.hpp"

Manager::Manager() {

}


void Manager::update() {
    for (auto& object: objects ) {
        object->early_update();
    }

    for (auto& object: objects ) {
        object->update();
    }

    for (auto& object: objects ) {
        object->late_update();
    }

    for (auto& object: objects ) {
        object->draw();
    }
}