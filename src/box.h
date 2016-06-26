#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include "obj.h"
#include "vec3.h"

class Box : public Obj {
public:
	Box(Vec3 corner_1, Vec3 corner_2, Vec3 col, float diffuse_c, float reflection_c, float specular_c) {
		if (corner_1[0] < corner_2[0]) {
			corners[0] = corner_1;
			corners[1] = corner_2;
		}
		else {
			corners[1] = corner_1;
			corners[0] = corner_2;
		}
		color = col;
		diffuse_coef = diffuse_c;
		reflection_coef = reflection_c;
		specular_coef = specular_c;

		x_min = std::min(corners[0][0], corners[1][0]);
		x_max = std::max(corners[0][0], corners[1][0]);
		y_min = std::min(corners[0][1], corners[1][1]);
		y_max = std::max(corners[0][1], corners[1][1]);
		z_min = std::min(corners[0][2], corners[1][2]);
		z_max = std::max(corners[0][2], corners[1][2]);
	}

	Vec3 normal(Vec3 point) {
		if (std::fabs(point[0] - x_min) < 0.001) {
			return Vec3(-1.0, 0.0, 0.0);
		}
		if (std::fabs(point[0] - x_max) < 0.001) {
			return Vec3(1.0, 0.0, 0.0);
		}
		if (std::fabs(point[1] - y_min) < 0.001) {
			return Vec3(0.0, -1.0, 0.0);
		}
		if (std::fabs(point[1] - y_max) < 0.001) {
			return Vec3(0.0, 1.0, 0.0);
		}
		if (std::fabs(point[2] - z_min) < 0.001) {
			return Vec3(0.0, 0.0, -1.0);
		}
		//if (std::fabs(point[2] - z_max) < 0.001) {
		else {
			return Vec3(0.0, 0.0, 1.0);
		}
	}

	float intersection(Ray ray) {
		float t;
		if (ray.direction[0] > 0) {
			t = (x_min - ray.origin[0]) / ray.direction[0];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[1] >= y_min && intersection[1] <= y_max &&
				intersection[2] >= z_min && intersection[2] <= z_max) {
				return t;
			}
		}
		else if (ray.direction[0] < 0) {
			t = (x_max - ray.origin[0]) / ray.direction[0];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[1] >= y_min && intersection[1] <= y_max &&
				intersection[2] >= z_min && intersection[2] <= z_max) {
				return t;
			}
		}
		if (ray.direction[1] > 0) {
			t = (y_min - ray.origin[1]) / ray.direction[1];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[0] >= x_min && intersection[0] <= x_max &&
				intersection[2] >= z_min && intersection[2] <= z_max) {
				return t;
			}
		}
		else if (ray.direction[1] < 0) {
			t = (y_max - ray.origin[1]) / ray.direction[1];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[0] >= x_min && intersection[0] <= x_max &&
				intersection[2] >= z_min && intersection[2] <= z_max) {
				return t;
			}
		}
		if (ray.direction[2] > 0) {
			t = (z_min - ray.origin[2]) / ray.direction[2];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[0] >= x_min && intersection[0] <= x_max &&
				intersection[1] >= y_min && intersection[1] <= y_max) {
				return t;
			}
		}
		else if (ray.direction[2] < 0) {
			t = (z_max - ray.origin[2]) / ray.direction[2];
			Vec3 intersection = ray.origin + ray.direction * t;
			if (intersection[0] >= x_min && intersection[0] <= x_max &&
				intersection[1] >= y_min && intersection[1] <= y_max) {
				return t;
			}
		}
		return -1.0; // No intersection
	}

	Vec3 corners[2];
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float z_min;
	float z_max;
};