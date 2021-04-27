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

bool refract(const vec3& incident, const vec3& normal, float eta, vec3& refracted) {
	vec3 I = normalize(incident);
	vec3 N = normal;
	float NdotI = dot(N, I);
	float k = 1.0 - eta * eta * (1.0 - NdotI * NdotI);
	if (k > 0) {
		refracted = (I - N * NdotI) * eta - N * sqrt(k);
		return true;
	}
	return false;
}


// Utility functions:
static unsigned int g_seed;
void rndSeed(int seed) { g_seed = seed; }
inline float rnd() {
	//return (float)rand() / (RAND_MAX + 1.0);
	g_seed = (214013 * g_seed + 2531011);
	return ((g_seed >> 16) & 0x7FFF) / 32767.0;
}

vec3 rndDirection() {
	vec3 p;
	do {
		p = vec3(rnd(), rnd(), rnd()) * 2.0 - vec3(1, 1, 1);
	} while (sqLength(p) >= 1.0);
	return p;
}

float schlickApproximation(float cosine, float ior) {
	float r0 = (1 - ior) / (1 + ior);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}