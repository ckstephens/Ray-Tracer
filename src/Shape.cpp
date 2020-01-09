#include "Shape.h"

Shape::Shape(void)
{
}

Shape::~Shape(void)
{
}

Shape::Shape(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s) :
	ka(ka), kd(kd), ks(ks), km(km), s(s)
{
}

void Shape::print() {};
