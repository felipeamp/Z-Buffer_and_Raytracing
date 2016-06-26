#include "stdafx.h"
#include "camera.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>

Ray Camera::ray(int point_x, int point_y) {
	/*std::cout << "Pontos x e y no plano near: (" << point_x << ", " << point_y << ")" << std::endl;
	std::cout << "x_eye = (" << x_eye[0] << ", " << x_eye[1] << ", " << x_eye[2] << ")" << std::endl;
	std::cout << "y_eye = (" << y_eye[0] << ", " << y_eye[1] << ", " << y_eye[2] << ")" << std::endl;
	std::cout << "z_eye = (" << z_eye[0] << ", " << z_eye[1] << ", " << z_eye[2] << ")" << std::endl;
	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "width = " << width << std::endl;
	std::cout << "height = " << height << std::endl;
	std::cout << "near = " << near << std::endl;
	Vec3 vecx = x_eye * b * ((static_cast<float> (point_x) / static_cast<float> (width)) - 0.5);
	Vec3 vecy = y_eye * a * ((static_cast<float> (point_y) / static_cast<float> (height)) - 0.5);
	Vec3 vecz = z_eye * (-near);
	Vec3 sum = vecx + vecy + vecz;
	std::cout << "x_eye * b * ((static_cast<float> (point_x) / static_cast<float> (width)) - 0.5) = " << std::endl <<
		"(" << vecx[0] << ", " << vecx[1] << ", " << vecx[2] << ")" << std::endl;
	std::cout << "y_eye * a * ((static_cast<float> (point_y) / static_cast<float> (height)) - 0.5) = " << std::endl <<
		"(" << vecy[0] << ", " << vecy[1] << ", " << vecy[2] << ")" << std::endl;
	std::cout << "z_eye * (-near) = " << std::endl <<
		"(" << vecz[0] << ", " << vecz[1] << ", " << vecz[2] << ")" << std::endl;
	std::cout << "sum of the above = " << "(" << sum[0] << ", " << sum[1] << ", " << sum[2] << ")" << std::endl;*/
	Vec3 ray_direction =
		x_eye * b * ((static_cast<float> (point_x) / static_cast<float> (width)) - 0.5)
		+ y_eye * a * ((static_cast<float> (point_y) / static_cast<float> (height)) - 0.5)
		+ z_eye * (-near);
	//std::cout << "Camera::ray direction result: (" << ray_direction[0] << ", " << ray_direction[1] << ", "
	//	<< ray_direction[2] << ")" << std::endl;
	//float var_inutil;
	//std::cin >> var_inutil;
	//std::cout << std::endl << std::endl << std::endl << std::endl;
	return Ray(eye, ray_direction);
}