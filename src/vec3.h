#pragma once

#include "gl/glut.h"

class Vec3 {
public:
	Vec3();
	Vec3(GLfloat, GLfloat, GLfloat);
	Vec3(GLfloat);


	Vec3 operator+ (Vec3&);
	Vec3 operator- (Vec3&);
	Vec3 operator- ();
	Vec3 operator* (GLfloat);
	Vec3 operator* (Vec3&);


	Vec3& operator= (const Vec3&);
	GLfloat& operator[] (size_t i);

	Vec3& operator+= (const Vec3&);
	Vec3& operator*= (const Vec3&);

	GLfloat len();
	Vec3 normalize();

	//GLfloat* to4();

	static Vec3 cross(Vec3, Vec3);
	static GLfloat dot(Vec3, Vec3);

	GLfloat vec[3];
};
