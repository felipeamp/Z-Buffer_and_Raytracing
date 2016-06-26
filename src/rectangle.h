#pragma once

#include <algorithm>
#include <cmath>
#include "obj.h"
#include "vec3.h"

class Rectangle : public Obj {
public:
	Rectangle(Vec3 corner_1, Vec3 corner_2, Vec3 corner_3, Vec3 corner_4,
			  Vec3 col, float diffuse_c, float reflection_c, float specular_c) {
		// We assume the corners were given in clockwise order.
		corners[0] = corner_1;
		corners[1] = corner_2;
		corners[2] = corner_3;
		corners[3] = corner_4;
		normal_vec = Vec3::cross(corner_4 - corner_1, corner_2 - corner_1).normalize();

		color = col;
		diffuse_coef = diffuse_c;
		reflection_coef = reflection_c;
		specular_coef = specular_c;
	}

	Vec3 normal(Vec3 point) {
		return this->normal_vec;
	}

	float intersection(Ray ray) {
		// Returns the distance to the intersection point. If negative, there is no intersection.
		float denom = Vec3::dot(ray.direction, this->normal_vec);
		if (std::abs(Vec3::dot(ray.direction, this->normal_vec)) < 0.000001) {
			return -1.0;
		}
		float distance_to_plane = Vec3::dot(corners[0] - ray.origin, this->normal_vec) / denom;
	    if (distance_to_plane <= 0) {
	    	// Point of intersection is in the negative direction of the ray.
	    	return -1.0;
	    }
	    // Let's check if the intersection point is inside the rectangle.
	    Vec3 intersection_with_plane = ray.origin + ray.direction * distance_to_plane;
	    for (size_t corner_index = 0; corner_index <= 3; ++corner_index) {
			Vec3 point_vector = intersection_with_plane - corners[corner_index];
			Vec3 side_vector;
	    	if (corner_index < 3) {
				side_vector = corners[corner_index + 1] - corners[corner_index];
	    	} else {
				side_vector = corners[0] - corners[corner_index];
	    	}
			Vec3 cross_product = Vec3::cross(point_vector, side_vector).normalize();
			float cross_dot_normal = Vec3::dot(cross_product, this->normal_vec);
	    	if (cross_dot_normal < 0) {
	    		// Intersection point with the rectangle plane is outside of the rectangle
	    		return -1.0;
	    	}
	    }
		return distance_to_plane;
	}

	Vec3 corners[4];
	Vec3 normal_vec;
};