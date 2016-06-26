#pragma once

#include <limits>
#include "vec3.h"

class Ray {
public:
	Ray(Vec3 o, Vec3 dir) {
        origin = o;
        direction = dir.normalize();
        object_index = -1;
        prev_object_index = -1;
        depth = 1;
        length = std::numeric_limits<float>::max();
    }
    Ray(Vec3 o, Vec3 dir, int dep) {
        origin = o;
        direction = dir.normalize();
        object_index = -1;
        prev_object_index = -1;
        depth = dep;
        length = std::numeric_limits<float>::max();
    }

	Vec3 origin;
	Vec3 direction;
    int object_index;
    int prev_object_index;
    float length;
    int depth;
};