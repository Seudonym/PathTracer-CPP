#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

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


const int imageWidth = 400, imageHeight = 400;
const int SPP = 300;
const float aspect = (float)imageWidth / (float)imageHeight;
float pixel[imageWidth][imageHeight][3];

Solid* list[5] = {
	new Sphere(vec3(0, -150.5, -2), 150, new Lambertian(vec3(0.4))),
	new Sphere(vec3(0.51, 0, -2), 0.5, new Lambertian(vec3(1.0, 0.0, 0.0))),
	new Sphere(vec3(-1.52, 0, -2), 0.5, new Reflective(vec3(1.0, 1.0, 1.0), 0.5)),
	new Sphere(vec3(-0.51, 0, -2), 0.5, new Transparent(1.4)),
	new Sphere(vec3(1.52, 0, -2), 0.5, new Reflective(vec3(0.8, 0.8, 0.8), 0)),
};
Solid* world = new World(list, 5);


void execThread(int y1, int y2, int x1, int x2) {
	vec3 col;
	float r, g, b;
	float u, v;
	Ray ray;
	
	for (int j = y1; j < y2; j++) {
		//std::cout << "\rRendering " << (int)(j * 100.0 / imageHeight + 1) << "% complete...			(" << j+1 << " of " << imageHeight << ")";
		for (int i = x1; i < x2; i++) {
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

			pixel[i][j][0] = r;
			pixel[i][j][1] = g;
			pixel[i][j][2] = b;

		
			//image << r << " " << g << " " << b << std::endl;
		}
	}


}


int main() {
	rndSeed(21221);
	const char* skybox = "skybox2.png";


	std::cout << "================================================\n";
	std::cout << "Width = " << imageWidth << "	" << "Height = " << imageHeight;
	std::cout << "\nAspect ratio = " << aspect;
	std::cout << "\nSamples per pixel = " << SPP;
	std::cout << "\nSkybox = " << skybox << std::endl;
	std::cout << "================================================\n";

	std::cout << "Loading skybox...\n";
	auto SKYBOX_START = std::chrono::high_resolution_clock::now();
	if (!loadImage(skybox)) {
		std::cout << "Failed to load skybox.\n";
	} else {
		std::cout << "Skybox loaded successfully in ";
		auto SKYBOX_END = std::chrono::high_resolution_clock::now();
		auto SKYBOX_ELAPSED = std::chrono::duration_cast<std::chrono::milliseconds>(SKYBOX_END - SKYBOX_START);
		std::cout << SKYBOX_ELAPSED.count() * 1e-3 << " seconds.\n";
	}
	std::cout << "Skybox resolution = " << w << "x" << h << std::endl;
	std::cout << "================================================\n";

	std::cout << "Rendering started..." << std::endl;
	auto RENDER_START = std::chrono::high_resolution_clock::now();

	std::thread t1(execThread, 0, imageHeight/2, 0, imageWidth/2);
	std::thread t2(execThread, 0, imageHeight/2, imageWidth/2, imageWidth);
	std::thread t3(execThread, imageHeight/2, imageHeight, 0, imageWidth / 2);
	std::thread t4(execThread, imageHeight/2, imageHeight, imageWidth / 2, imageWidth);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	

	auto RENDER_END = std::chrono::high_resolution_clock::now();
	auto RENDER_ELAPSED = std::chrono::duration_cast<std::chrono::milliseconds>(RENDER_END - RENDER_START);
	std::cout << "\nRendering finished in " << RENDER_ELAPSED.count() * 1e-3 << " seconds.\n";

	auto WRITE_START = std::chrono::high_resolution_clock::now();
	std::cout << "Writing render to image...\n";
	std::ofstream image("image.ppm");
	image << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
	for (int j = 0; j < imageHeight; j++) {
		for (int i = 0; i < imageWidth; i++) {
			image << pixel[i][j][0] << " " << pixel[i][j][1] << " " << pixel[i][j][2] << "\n";
		}
	}
	std::cout << "Write complete in" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - WRITE_START).count()*1e-3;

	return 0;
}