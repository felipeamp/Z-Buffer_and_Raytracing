#pragma once

#include <cmath>
#include "ray.h"
#include "vec3.h"
#include <iostream>

const float PI = 2.0 * std::acos(0.0);

class Camera {
public:
	Camera() {};
    Camera(Vec3 e, Vec3 c, Vec3 up_vector, float fov_angle, float n, float f, int w, int h) {
        eye = e;
        center = c;
        up = up_vector;
        fov = fov_angle;
        near = n;
        far = f;
        width = w;
        height = h;

        a = 2 * near * std::tan(std::abs(fov * PI/180.0)/2.0);
		b = a * static_cast<float> (width) / static_cast<float> (height);

        z_eye = (eye - center).normalize();
        x_eye = Vec3::cross(up, z_eye).normalize();
        y_eye = Vec3::cross(z_eye, x_eye);
    }

	Ray ray(int, int);

    Vec3 eye;
    Vec3 center;
    Vec3 up;
    float fov;
    float near;
    float far;
    int width;
    int height;

    Vec3 x_eye;
    Vec3 y_eye;
    Vec3 z_eye;
    float a;
    float b;
};