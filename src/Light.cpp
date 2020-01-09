#include "Light.h"

Light::Light(void)
{
}

Light::~Light(void)
{
}

Light::Light(glm::vec3 p, glm::vec3 c, bool a, float w, float h, float l) : position(p), color(c), area(a), width(w), height(h), length(l)
{
}
