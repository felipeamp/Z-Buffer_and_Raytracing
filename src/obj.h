#pragma once

#include "ray.h"
#include "vec3.h"

class Obj {
public:
	Vec3 color;
	float diffuse_coef;
    float reflection_coef;
    float specular_coef;
	virtual Vec3 normal(Vec3) = 0;
	virtual float intersection(Ray) = 0;
};
