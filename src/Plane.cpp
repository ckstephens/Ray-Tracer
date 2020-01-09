#include "Plane.h"

Plane::Plane()
{
}

Plane::~Plane()
{
}

Plane::Plane(glm::vec3 c, glm::vec3 n, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s) :
	center(c), normal(n), Shape(ka, kd, ks, km, s)
{
}

