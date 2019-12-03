#pragma once
#include "Vector.h"

float getRandom() {
	return (float)rand() / ((float)RAND_MAX + 1.0f);
}

Vector getRandomInCircle() {
	Vector v;
	do {
		v = 2.0f * Vector(getRandom(), getRandom(), 0.0f) - Vector(1.0f, 1.0f, 0.0f);
	} while (v.squaredLength() >= 1.0f);
	return v;
}

Vector getRandomInSphere() {
	Vector v;
	do {
		v = 2.0f * Vector(getRandom(), getRandom(), getRandom()) - Vector(1.0f, 1.0f, 1.0f);
	} while (v.squaredLength() >= 1.0f);
	return v;
}