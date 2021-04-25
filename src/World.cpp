#include "World.h"

World::World() {}
World::World(Solid** _solidList, int _listSize) : solidList(_solidList), listSize(_listSize) {}

bool World::intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection) {
	Intersection temp;
	bool didIntersect = false;
	float tClosest = tMax;
	for (int i = 0; i < listSize; i++) {
		if (solidList[i]->intersect(ray, tMin, tClosest, temp)) {
			didIntersect = true;
			tClosest = temp.t;
			intersection = temp;
		}
	}
	return didIntersect;
}