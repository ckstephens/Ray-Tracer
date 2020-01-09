#pragma once
#include <glm/gtc/type_ptr.hpp>

class Shape
{
public:
	Shape(void);
	~Shape(void);
	Shape(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s);

	virtual void print();

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float s;
};
