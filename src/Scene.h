#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	~Scene();

	void addShape(Shape* s);
	void addLight(Light* l);

	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
