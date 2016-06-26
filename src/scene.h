#pragma once

#include <memory>
#include <vector>
#include "camera.h"
#include "light.h"
#include "obj.h"
#include "vec3.h"


class Scene {
public:
	Scene() {};
	Vec3 ambient_color;
	Camera camera;
	std::vector<Light> lights;
	std::vector<std::shared_ptr<Obj>> objects;
};