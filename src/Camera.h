#pragma once
#include <vector>
#include "Scene.h"


class Camera
{
public:
	Camera();
	~Camera();

	Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fovy, float focal, int widthRes, int heightRes);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };

private:
	glm::vec3 eye;
	glm::vec3 lookAt;
	glm::vec3 up;

	float fovy;
	float focal;
	int widthRes;
	int heightRes;

	float *depth;
	float *sbuff;
	float *renderedImage;

	glm::vec3 ComputeRayColor(glm::vec3& origin, glm::vec3& direction, std::vector<Shape*>& shapes, std::vector<Light*>& lights, int r_count);
};
