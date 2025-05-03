#pragma once

#include <stdfloat>
#include <iostream>

class Object {
    friend class Manager;

    protected:
        virtual void early_update();
        virtual void update();
        virtual void late_update();
        virtual void draw();

        bool enabled;
};