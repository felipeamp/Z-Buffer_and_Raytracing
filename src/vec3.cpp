#include "stdafx.h"
#include <cmath>
#include "vec3.h"
#include <iostream>

Vec3::Vec3() {
	vec[0] = 0.0;
	vec[1] = 0.0;
	vec[2] = 0.0;
}

Vec3::Vec3(GLfloat x, GLfloat y, GLfloat z) {
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

Vec3::Vec3(GLfloat f) {
	vec[0] = f;
	vec[1] = f;
	vec[2] = f;
}

Vec3 Vec3::operator+ (Vec3& other) {
	return Vec3(
		vec[0] + other[0],
		vec[1] + other[1],
		vec[2] + other[2]);
}

Vec3 Vec3::operator- (Vec3& other) {
	return Vec3(
		vec[0] - other[0],
		vec[1] - other[1],
		vec[2] - other[2]);
}

Vec3 Vec3::operator- () {
	return Vec3(- vec[0], - vec[1], - vec[2]);
}

Vec3 Vec3::operator* (Vec3& other) {
	return Vec3(
		vec[0] * other[0],
		vec[1] * other[1],
		vec[2] * other[2]);
}

Vec3 Vec3::operator* (GLfloat f) {
	return Vec3(
		vec[0] * f,
		vec[1] * f,
		vec[2] * f);
}

Vec3& Vec3::operator= (const Vec3& rhs) {
	vec[0] = rhs.vec[0];
	vec[1] = rhs.vec[1];
	vec[2] = rhs.vec[2];
	return *this;
}

GLfloat& Vec3::operator[] (size_t i) {
	return vec[i];
}

Vec3& Vec3::operator+= (const Vec3& rhs) {
	vec[0] += rhs.vec[0];
	vec[1] += rhs.vec[1];
	vec[2] += rhs.vec[2];

	return *this;
}

Vec3& Vec3::operator*= (const Vec3& rhs) {
	vec[0] *= rhs.vec[0];
	vec[1] *= rhs.vec[1];
	vec[2] *= rhs.vec[2];

	return *this;
}

GLfloat Vec3::len() {
	return std::sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

Vec3 Vec3::normalize() {
	GLfloat len = this->len();
	if (len > 0) { // avoids normalization of zero vectors
		vec[0] /= len;
		vec[1] /= len;
		vec[2] /= len;
	}
	return *this;
}

Vec3 Vec3::cross(Vec3 v1, Vec3 v2) {
	/*std::cout << "v1 = (" << v1[0] << ', ' << v1[1] << ', ' << v1[2] << ')' << std::endl;
	std::cout << "v2 = (" << v2[0] << ', ' << v2[1] << ', ' << v2[2] << ')' << std::endl;
	std::cout << "first coord = " << v1[1] * v2[2] - v2[2] * v1[1] << std::endl;
	std::cout << "second coord = " << v1[2] * v2[0] - v2[0] * v1[2] << std::endl;
	std::cout << "third coord = " << v1[1] * v2[2] - v2[2] * v1[1] << std::endl;*/
	return Vec3(
		v1[1] * v2[2] - v2[1] * v1[2],
		v1[2] * v2[0] - v2[2] * v1[0],
		v1[0] * v2[1] - v2[0] * v1[1]);
}

GLfloat Vec3::dot(Vec3 v1, Vec3 v2) {
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}