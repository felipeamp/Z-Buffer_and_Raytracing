#pragma once

#include "vec3.h"

class Light {
public:
    Light(Vec3 c, Vec3 p) {
        color = c;
        position = p;
    }

    Vec3 color;
    Vec3 position;
};
