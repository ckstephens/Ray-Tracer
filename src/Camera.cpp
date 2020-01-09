#include "Camera.h"
#include <algorithm>

// Functions to find the index of 3D and 2D arrays if the array is given as a pointer to a type
#define INDEX3D(h, d, x, y, z) (int)(x * h * d + y * d + z)
#define INDEX2D(h, x, y) (int)(x * h + y)


Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fovy, float focal, int widthRes, int heightRes) :
	eye(eye), lookAt(lookAt), up(up), fovy(fovy), focal(focal), widthRes(widthRes), heightRes(heightRes)
{
	depth = new float[widthRes * heightRes];
	sbuff = new float[widthRes * heightRes];
	renderedImage = new float[widthRes * heightRes * 3];
}

// Epsilon
#define E_L 1e-4f

glm::vec3 Camera::ComputeRayColor(glm::vec3& o, glm::vec3& d, std::vector<Shape*>& shapes, std::vector<Light*>& lights, int r_count) {
	float tmin = FLT_MAX;
	int closest = -1;
	for (int i = 0; i < shapes.size(); i++) {
		// Find intersections if plane
		if (Plane* plane = dynamic_cast<Plane*>(shapes.at(i))) {
			float t = glm::dot((plane->center - o), plane->normal) / glm::dot(d, plane->normal);
			if (t < E_L) continue;
			if (t < tmin) {
				tmin = t;
				closest = i;
			}
		}

		// Find intersections if sphere
		else if (Sphere* sphere = dynamic_cast<Sphere*>(shapes.at(i))) {
			glm::vec3 oc = o - sphere->position;
			float a = glm::dot(d, d);
			float b = glm::dot((2.0f * oc), d);
			float c = glm::dot(oc, oc) - sphere->radius * sphere->radius;
			float disc = b * b - 4.0f * a * c;
			if (disc < 1e-4f) continue;

			float t1 = (-b + sqrt(disc)) / (2.0f * a);
			float t2 = (-b - sqrt(disc)) / (2.0f * a);
			float t = (t1 < t2) ? t1 : t2;
			if (t < E_L) continue;
			if (t < tmin) {
				tmin = t;
				closest = i;
			}
		}
	}

	if (closest == -1 || tmin < E_L) return glm::vec3(0.0f);

	glm::vec3 p = o + tmin * d;
	if (Plane* plane = dynamic_cast<Plane*>(shapes.at(closest))) {
		glm::vec3 color = plane->ka;
		glm::vec3 E = glm::normalize(eye - p);

		// Find shadows on plane
		std::vector<bool> shadowed(lights.size());
		std::fill(shadowed.begin(), shadowed.end(), false);
		for (int l = 0; l < lights.size(); l++) {
			for (int s = 0; s < shapes.size(); s++) {
				if (Sphere* sphere_shd = dynamic_cast<Sphere*>(shapes.at(s))) {
					glm::vec3 pd;
					// Area lighting --- Pick a random point
					if (lights.at(l)->area) {
						pd = glm::vec3(
							(float)(lights.at(l)->position[0] + ((float)(rand() % (((int)(lights.at(l)->width * 10000.0f)))) - (float)(lights.at(l)->width * 5000.0f))/10000.0f),
							(float)(lights.at(l)->position[1] + ((float)(rand() % (((int)(lights.at(l)->height * 10000.0f)))) - (float)(lights.at(l)->height * 5000.0f))/10000.0f),
							(float)(lights.at(l)->position[2] + ((float)(rand() % (((int)(lights.at(l)->length * 10000.0f)))) - (float)(lights.at(l)->length* 5000.0f))/10000.0f)
						);
					}
					// Normal lighting
					else {
						pd = glm::normalize(lights.at(l)->position - p);
					}
					float a = glm::dot(pd, pd);
					glm::vec3 po = p - sphere_shd->position;
					float b = glm::dot((2.0f * po), pd);
					float c = glm::dot(po, po) - sphere_shd->radius * sphere_shd->radius;
					float disc = b * b - 4.0f * a * c;
					if (disc < E_L) continue;

					float t1 = (-b + sqrt(disc)) / (2.0f * a);
					float t2 = (-b - sqrt(disc)) / (2.0f * a);
					float t = (t1 < t2) ? t1 : t2;
					if (t < E_L) continue;
					if (glm::length((p + t * pd) - p) < glm::length(lights.at(l)->position - p)) {
						shadowed.at(l) = true;
						if (lights.at(l)->area) color *= glm::length(p - sphere_shd->position) / sphere_shd->radius;
					}
				}
			}
		}

		// Shade plane
		for (int i = 0; i < lights.size(); i++) {
			if (!shadowed.at(i)) {
				glm::vec3 L = glm::normalize(lights.at(i)->position - p);
				glm::vec3 R = glm::normalize(glm::reflect(-L, plane->normal));
				float diffuse = std::max(glm::dot(L, plane->normal), 0.0f);
				float specular = std::max(dot(R, E), 0.0f);
				color += lights.at(i)->color * (plane->kd * diffuse + plane->ks * pow(specular, plane->s));
			}
		}
		return color;
	}

	if (Sphere* sphere = dynamic_cast<Sphere*>(shapes.at(closest))) {
		// Recursive reflection
		if (sphere->reflective) {
			if (r_count > 30) {
				r_count = 0;
			}
			else {
				r_count++;
				glm::vec3 rd = glm::normalize(glm::reflect(d, glm::normalize(p - sphere->position)));
				return sphere->km * ComputeRayColor(p, rd, shapes, lights, r_count);
			}
		}

		// Shade sphere
		glm::vec3 color = sphere->ka;
		glm::vec3 E = glm::normalize(o - p);
		glm::vec3 N = glm::normalize(p - sphere->position);

		for (int i = 0; i < lights.size(); i++) {
			glm::vec3 L = glm::normalize(lights.at(i)->position - p);
			glm::vec3 R = glm::normalize(glm::reflect(-L, N));
			float diffuse = std::max(glm::dot(L, N), 0.0f);
			float specular = std::max(dot(R, E), 0.0f);
			color += lights.at(i)->color * (sphere->kd * diffuse + sphere->ks * pow(specular, sphere->s));
		}
		return color;
	}
	return glm::vec3(0.0f);
}

void Camera::TakePicture(Scene *scene)
{
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);

	float aspect = (float)widthRes / (float)heightRes;
	float t = focal * tanf((glm::pi<float>() * fovy) / 360.0f);
	float b = -t;
	float r = t * aspect;
	float l = -r;
	glm::vec3 camw = glm::normalize(eye - lookAt);
	glm::vec3 camu = glm::normalize(glm::cross(up, camw));
	glm::vec3 camv = glm::cross(camw, camu);
	glm::vec3 color(0.4f);

	for (int h = 0; h < heightRes; h++) {
		for (int w = 0; w < widthRes; w++) {
			float u = l + (r - l) * ((float)w + 0.5f) / (float)widthRes;
			float v = b + (t - b) * ((float)h + 0.5f) / (float)heightRes;
			glm::vec3 direction = -focal * camw + u * camu + v * camv;
			color = ComputeRayColor(eye, direction, scene->shapes, scene->lights, 0);
			for (int c = 0; c < 3; c++) {
				renderedImage[INDEX3D(widthRes, 3, h, w, c)] = color[c];
			}
		}
	}
}
