#include "stdafx.h"
#include "camera.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>

Ray Camera::ray(int point_x, int point_y) {
    Vec3 ray_direction =
		x_eye * b * ((static_cast<float> (point_x) / static_cast<float> (width)) - 0.5)
		+ y_eye * a * ((static_cast<float> (point_y) / static_cast<float> (height)) - 0.5)
		+ z_eye * (-near);
	return Ray(eye, ray_direction);
}