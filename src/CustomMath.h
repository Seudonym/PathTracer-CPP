#ifndef __CUSTOMMATH__
#define __CUSTOMMATH__

#include <iostream>
#include <cmath>



class vec3 {
public:
	float x, y, z;

	vec3();
	vec3(float _x, float _y, float _z);
	vec3(float a);
};

vec3 operator+(const vec3& a, const vec3& b);
vec3 operator-(const vec3& a, const vec3& b);
vec3 operator*(const vec3& a, const vec3& b);

vec3 operator*(const vec3& a, float number);
vec3 operator/(const vec3& a, float number);

std::ostream& operator<<(std::ostream& os, const vec3& a);

float sqLength(const vec3& a);
float length(const vec3& a);
vec3 normalize(const vec3& a);
float dot(const vec3& a, const vec3& b);
vec3 cross(const vec3& a, const vec3& b);

class Ray {
public:
	vec3 origin, direction;

	Ray();
	Ray(const vec3& _origin, const vec3& _direction);

	
};

vec3 pointOnRay(const Ray& ray, float t);
vec3 reflect(const vec3& incident, const vec3& normal);
bool refract(const vec3& incident, const vec3& normal, float eta, vec3& refracted);

void rndSeed(int seed);
inline float rnd();
vec3 rndDirection();
float schlickApproximation(float cosine, float ior);
#endif