#ifndef __SOLID__
#define __SOLID__

#include "CustomMath.h"

struct Intersection;

class Material {
public:
	virtual bool scatter(const Ray& ray, const Intersection& intersection, vec3& attenuation, Ray& scattered) = 0;
};

struct Intersection {
public:
	float t;
	vec3 point;
	vec3 normal;
	Material* material;
};

class Lambertian : public Material {
public:
	vec3 albedo;
	Lambertian(const vec3& _albedo);
	bool scatter(const Ray& ray, const Intersection& intersection, vec3& attenuation, Ray& scattered);
};

class Reflective : public Material {
public:
	vec3 albedo;
	float fuzziness;
	Reflective(const vec3& _albedo, float _fuzziness);
	bool scatter(const Ray& ray, const Intersection& intersection, vec3& attenuation, Ray& scattered);
};

class Transparent : public Material {
public:
	float ior;
	Transparent(float _ior);
	bool scatter(const Ray& ray, const Intersection& intersection, vec3& attenuation, Ray& scattered);
};

class Solid {
public:
	virtual bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection) = 0;
};

class Sphere : public Solid {
public:
	vec3 center;
	float radius;
	Material* material;

	Sphere();
	Sphere(const vec3& _center, float _radius, Material* _material);

	bool intersect(const Ray& ray, float tMin, float tMax, Intersection& intersection);
};

#endif