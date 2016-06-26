#pragma once

#include <algorithm>
#include <cmath>
#include "obj.h"
#include "vec3.h"

class Sphere : public Obj {
public:
	Sphere(Vec3 cen, float rad, Vec3 col, float diffuse_c, float reflection_c, float specular_c) {
		center = cen;
		radius = rad;

		color = col;
		diffuse_coef = diffuse_c;
		reflection_coef = reflection_c;
		specular_coef = specular_c;
	}

	Vec3 normal(Vec3 point) {
		return (point - center).normalize();
	}

	float intersection(Ray ray) {
		// Returns the distance to the intersection point. If negative, there is no intersection.

		float a = Vec3::dot(ray.direction, ray.direction);
		float b = 2 * Vec3::dot(ray.direction, ray.origin - center);
		float c = Vec3::dot(ray.origin - center, ray.origin - center) - radius * radius;

		float delta = b * b - 4 * a * c;

		if (delta > 0) { // There are 2 intersection points!
			float t_1 = (-b + std::sqrt(delta)) / (2 * a);
			float t_2 = (-b - std::sqrt(delta)) / (2 * a);

			return std::min(t_1, t_2);
		}
		return -1.0;
	}

	Vec3 center;
	float radius;
};
