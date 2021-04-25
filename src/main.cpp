#include <iostream>
#include <fstream>
#include <vector>

#include "CustomMath.h"
#include "Solid.h"
#include "World.h"

#define PI 3.1415926


#pragma region SKYBOX_LOADER
extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"
}

std::vector<unsigned char> imgData;
int w, h, n;
bool loadImage(const char* filename) {
	unsigned char* data;
	data = stbi_load(filename, &w, &h, &n, 0);
	bool read = (data != nullptr);
	if (read) imgData = std::vector<unsigned char>(data, data + w * h * n);
	stbi_image_free(data);
	return read;
}

vec3 skybox(const vec3& rayDirection) {
	float u = (0.5 + atan2(rayDirection.x, rayDirection.z) / (2.0 * PI));
	float v = (0.5 - asin(rayDirection.y) / PI);

	int x = (int)(u * (float)w);
	int y = (int)(v * (float)h);
	int index = n * (y * w + x);

	int r = static_cast<int>(imgData[index]);
	int g = static_cast<int>(imgData[index+1]);
	int b = static_cast<int>(imgData[index+2]);

	return vec3(r, g, b) / 255.0;
}
#pragma endregion SKYBOX_LOADER


vec3 color(Ray& ray, Solid* world, int depth) {
	
	Intersection intersection;
	Ray scattered;
	vec3 attenuation;
	if (world->intersect(ray, 0, INFINITY, intersection)) {
		if (depth < 50 && intersection.material->scatter(ray, intersection, attenuation, scattered))
			return attenuation * color(scattered, world, depth + 1);
		else return vec3(0);
	}
	else {
		/*float y = normalize(ray.direction).y;
		float t = (y + 1.0f) * 0.5f;
		return (1.0f - t) * vec3(1.0f) + t * vec3(0.5f, 0.7f, 1.0f);*/
		return skybox(normalize(ray.direction));
	}
}

int main() {

	if (!loadImage("skybox.png")) {
		std::cout << "Failed to load skybox.\n";
	}
	else { std::cout << "Skybox loaded.\n"; }

	int imageWidth = 1280, imageHeight = 720;
	int SPP = 275;
	float aspect = (float)imageWidth / (float)imageHeight;

	std::ofstream image("image.ppm");
	image << "P3\n" << imageWidth << " " << imageHeight  << " 255\n" << std::endl;

	vec3 col;
	float r, g, b;
	float u, v;
	Ray ray;

	Solid* list[4] = {
		new Sphere(vec3(0, -100.25, -1), 100, new Lambertian(vec3(0.4))),
		new Sphere(vec3(-0.5, 0, -1), 0.25, new Lambertian(vec3(1.0, 1.0, 1.0))),
		new Sphere(vec3(0, 0, -1), 0.25, new Metal(vec3(1.0, 1.0, 1.0), 0.5)),
		new Sphere(vec3(0.5, 0, -1), 0.25, new Metal(vec3(0.8, 0.8, 0.8), 0)),
	};
	Solid* world = new World(list, 4);


	for (int j = 0; j < imageHeight; j++) {
		std::cout << j << std::endl;
		for (int i = 0; i < imageWidth; i++) {
			
			col = vec3(0.0);
			for (int s = 0; s < SPP; s++) {
				
				u = (float)(i + rnd()) * 2.0 / imageWidth - 1.0;
				v = 1.0 - (float)(j + rnd()) * 2.0 / imageHeight;
				u = u * aspect;

				ray = Ray(vec3(0.0), vec3(u, v, -1.0));
				col = col + color(ray, world, 0);
			}
			col = col / (float)SPP;

			col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			r = col.x * 255.99, g = col.y * 255.99, b = col.z * 255.99;
			image << r << " " << g << " " << b << std::endl;
		}
	}


	return 0;
}