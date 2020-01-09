#pragma once
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	~Light();

	Light(glm::vec3 position, glm::vec3 color, bool area = false, float width = 0.0f, float height = 0.0f, float length = 0.0f);

	glm::vec3 position;
	glm::vec3 color;

	bool area;
	float width;
	float height;
	float length;
};
