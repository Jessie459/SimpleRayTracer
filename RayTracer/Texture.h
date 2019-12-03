#pragma once
#include "Vector.h"

class Texture {
public:
	virtual Vector value(float u, float v, const Vector &p) const = 0;
};

class ConstantTexture : public Texture {
private:
	Vector color;
public:
	ConstantTexture(const Vector &color) {
		this->color = color;
	}
	virtual Vector value(float u, float v, const Vector &p) const override {
		return color;
	}
};

class CheckerTexture : public Texture {
private:
	Texture *texture1;
	Texture *texture2;
public:
	CheckerTexture(Texture *texture1, Texture *texture2) {
		this->texture1 = texture1;
		this->texture2 = texture2;
	}
	virtual Vector value(float u, float v, const Vector &p) const override {
		float res = sin(10.0f * p.x()) * sin(10.0f * p.y()) * sin(10.0f * p.z());
		if (res < 0)
			return texture1->value(u, v, p);
		else
			return texture2->value(u, v, p);
	}
};

class ImageTexture : public Texture {
private:
	int nx, ny;
	unsigned char *data;
public:
	ImageTexture(unsigned char *data, int nx, int ny) {
		this->data = data;
		this->nx = nx;
		this->ny = ny;
	}
	virtual Vector value(float u, float v, const Vector &p) const override {
		int x = (int)(u * nx);
		int y = (int)(v * ny);
		if (x < 0)	x = 0;
		if (y < 0)	y = 0;
		if (x > nx - 1)	x = nx - 1;
		if (y > ny - 1)	y = ny - 1;
		float r = int(data[3 * x + 3 * nx * y + 0]) / 255.0f;
		float g = int(data[3 * x + 3 * nx * y + 1]) / 255.0f;
		float b = int(data[3 * x + 3 * nx * y + 2]) / 255.0f;
		return Vector(r, g, b);
	}
};