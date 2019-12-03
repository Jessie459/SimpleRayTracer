#pragma once
#include <math.h>
#include "Ray.h"
#include "Random.h"
#include "Vector.h"

class Camera {
private:
	Vector origin;
	Vector u, v, w;
	Vector lowerLeftCorner;
	Vector horizontal;
	Vector vertical;
	float radius;

public:
	Camera(Vector origin, Vector target, Vector up, float fov, float aspect, float aperture, float distance) {
		// set origin
		this->origin = origin;

		// set camera coordinate w, u, v
		float angle = fov * PI / 180.0f;
		float halfHeight = tan(angle / 2.0f);
		float halfWidth = aspect * halfHeight;
		w = (origin - target).normalized();
		u = cross(up, w).normalized();
		v = cross(w, u);

		// set lower left corner, horizontal, vertical
		lowerLeftCorner = origin - halfWidth * distance * u - halfHeight * distance * v - distance * w;
		horizontal = 2 * halfWidth * distance * u;
		vertical = 2 * halfHeight * distance * v;

		// set lens radius
		radius = aperture / 2.0f;
	}

	Ray getRay(float i, float j) {
		Vector offset = radius * getRandomInCircle();
		offset = offset.x() * u + offset.y() * v; // project to camera coordinate
		return Ray(origin + offset, lowerLeftCorner + i * horizontal + j * vertical - origin - offset);
	}
};