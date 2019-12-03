#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "AABB.h"
#include "BVH.h"
#include "Camera.h"
#include "ConstantMedium.h"
#include "Cuboid.h"
#include "Hittable.h"
#include "Material.h"
#include "Random.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Texture.h"
#include "Transform.h"
#include "Triangle.h"
#include "Vector.h"
using namespace std;

Vector color(const Ray &ray, Hittable *objects, int depth = 0) {
	HitInfo info;
	if (objects->hit(ray, 0.01f, FLT_MAX, info)) {
		Vector emittedColor = info.material->emit(info.u, info.v, info.position);
		Vector attenuation;
		Ray scatteredRay;
		if (depth < 50 && info.material->scatter(ray, info, attenuation, scatteredRay))
			return emittedColor + attenuation * color(scatteredRay, objects, depth + 1);
		else
			return emittedColor;
	}
	else
		return Vector(0.0f, 0.0f, 0.0f);
}

Hittable *cornellBox() {
	Material *red = new Lambertian(new ConstantTexture(Vector(0.65f, 0.05f, 0.05f)));
	Material *green = new Lambertian(new ConstantTexture(Vector(0.12f, 0.45f, 0.15f)));
	Material *white = new Lambertian(new ConstantTexture(Vector(0.73f, 0.73f, 0.73f)));
	Material *light = new Diffuse(new ConstantTexture(Vector(15.0f, 15.0f, 15.0f)));

	Hittable **list = new Hittable *[13];
	int i = 0;
	list[i++] = new YZRectangle(0, 555, 0, 555, 555, green, true);
	list[i++] = new YZRectangle(0, 555, 0, 555, 0, red, false);
	list[i++] = new XZRectangle(0, 555, 0, 555, 555, white, true);
	list[i++] = new XZRectangle(0, 555, 0, 555, 0, white, false);
	list[i++] = new XYRectangle(0, 555, 0, 555, 555, white, true);
	list[i++] = new XZRectangle(213, 343, 227, 332, 554, light, false);
	list[i++] = new Translate(new RotateAroundY(new Cuboid(Vector(0, 0, 0), Vector(165, 330, 165), white),  15), Vector(265, 0, 295));
	list[i++] = new Translate(new RotateAroundY(new Cuboid(Vector(0, 0, 0), Vector(165, 165, 165), white), -18), Vector(130, 0,  65));
	list[i++] = new Sphere(Vector(350, 380, 380), 50, new Metal(Vector(0.8f, 0.8f, 0.9f), 1.0f));
	list[i++] = new Sphere(Vector(380, 50, 100), 50, new Dielectric(1.0f, 1.5f));
	list[i++] = new Triangle(Vector(555, 200, 270), Vector(500, 0, 10), Vector(430, 0, 300), white);

	/*int nx, ny, comp;
	unsigned char* data = stbi_load("resources/earthmap.jpg", &nx, &ny, &comp, 0);
	list[i++] = new Sphere(Vector(100, 350, 400), 50, new Lambertian(new ImageTexture(data, nx, ny)));*/

	Hittable* boundary = new Sphere(Vector(212, 215, 148), 50, new Dielectric(1.0f, 1.5f));
	list[i++] = boundary;
	list[i++] = new ConstantMedium(boundary, 0.2f, new ConstantTexture(Vector(0.2f, 0.4f, 0.9f)));

	return new HittableList(list, i);
}

int main() {
	int nx = 800; // number of pixels in x direction
	int ny = 800; // number of pixels in y direction
	int ns = 10000; // number of samples per pixel

	// set hittable objects
	Hittable* objects = cornellBox();

	// set camera parameters
	Vector origin(278.0f, 278.0f, -800.0f);
	Vector target(278.0f, 278.0f, 0.0f);
	Vector up(0.0f, 1.0f, 0.0f);
	float fov = 40.0f;
	float aperture = 0.0f;
	float distance = 10.0f;
	Camera camera(origin, target, up, fov, float(nx) / float(ny), aperture, distance);

	// write header to ppm file
	std::ofstream os;
	os.open("images/image3.ppm");
	os << "P3\n" << nx << " " << ny << "\n255\n";

	// write data to ppm file
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			Vector c(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++) {
				float u = float(i + getRandom()) / float(nx);
				float v = float(j + getRandom()) / float(ny);
				Ray ray = camera.getRay(u, v);
				c += color(ray, objects);
			}
			c /= float(ns);
			c = Vector(sqrt(c[0]), sqrt(c[1]), sqrt(c[2]));
			int r = int(255.99f * c[0]);
			int g = int(255.99f * c[1]);
			int b = int(255.99f * c[2]);
			os << r << " " << g << " " << b << "\n";
		}
	}

	os.close();
	return 0;
}