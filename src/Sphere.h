#pragma once
#include "Shape.h"


class Sphere :
	public Shape
{
public:
	Sphere();
	~Sphere();
	Sphere(glm::vec3 position, float radius, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s, bool reflective);

	glm::vec3 position;
	float radius;
	bool reflective;
};
