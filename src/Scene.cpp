#include "Scene.h"

Scene::~Scene()
{
}

Scene::Scene()
{
}

void Scene::addShape(Shape* s) {
	shapes.push_back(s);
}

void Scene::addLight(Light* l) {
	lights.push_back(l);
}
