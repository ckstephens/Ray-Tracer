#include "Sphere.h"

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

Sphere::Sphere(glm::vec3 p, float r, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s, bool rf) :
	position(p), radius(r), reflective(rf), Shape(ka, kd, ks, km, s)
{
}
