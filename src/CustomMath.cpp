#include "CustomMath.h"


// Vector class:
// ===========================================================================================
vec3::vec3() : x(0), y(0), z(0) {}
vec3::vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
vec3::vec3(float a) : x(a), y(a), z(a) {}

vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
vec3 operator-(const vec3& a, const vec3& b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
vec3 operator*(const vec3& a, const vec3& b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }

vec3 operator*(const vec3& a, float number) { return vec3(a.x*number, a.y*number, a.z*number); }
vec3 operator/(const vec3& a, float number) { return vec3(a.x / number, a.y / number, a.z / number); }

std::ostream& operator<<(std::ostream& os, const vec3& a) { os << "(" << a.x << ", " << a.y << ", " << a.z << ")"; return os; }

float sqLength(const vec3& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
float length(const vec3& a) { return sqrt(a.x* a.x + a.y * a.y + a.z * a.z); }
vec3 normalize(const vec3& a) { return vec3(a.x, a.y, a.z) / length(a); }
float dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 cross(const vec3& a, const vec3& b) {
	return vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

// Ray class:

Ray::Ray(){}
Ray::Ray(const vec3& _origin, const vec3& _direction):origin(_origin), direction(_direction){}

vec3 pointOnRay(const Ray& ray, float t) { return ray.origin + ray.direction * t; }

vec3 reflect(const vec3& incident, const vec3& normal) { return incident - normal * 2.0 * dot(incident, normal); }

// Utility functions:
float rnd() {
	return (float)rand() / (RAND_MAX + 1.0);
}

vec3 rndDirection() {
	vec3 p;
	do {
		p = vec3(rnd(), rnd(), rnd()) * 2.0 - vec3(1, 1, 1);
	} while (sqLength(p) >= 1.0);
	return p;
}
