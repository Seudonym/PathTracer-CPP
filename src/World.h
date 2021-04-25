#ifndef __WORLD__
#define __WORLD__

#include "Solid.h"

class World : public Solid {
public:
	Solid** solidList;
	int listSize;

	World();
	World(Solid** _solidList, int _listSize);

	bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection);
};

#endif
