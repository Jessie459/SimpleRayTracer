#pragma once
#include "Hittable.h"

class XYRectangle : public Hittable {
private:
	float x0, x1, y0, y1, z;
	Material *material;
	bool flip;
public:
	XYRectangle(float x0, float x1, float y0, float y1, float z, Material *material, bool flip = false) {
		this->x0 = x0;
		this->x1 = x1;
		this->y0 = y0;
		this->y1 = y1;
		this->z = z;
		this->material = material;
		this->flip = flip;
	}
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		float distance = (z - ray.origin().z()) / ray.direction().z();
		if (distance < minDistance || distance > maxDistance)
			return false;
		float x = ray.origin().x() + distance * ray.direction().x();
		float y = ray.origin().y() + distance * ray.direction().y();
		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;
		info.u = (x - x0) / (x1 - x0);
		info.v = (y - y0) / (y1 - y0);
		info.distance = distance;
		info.position = ray.positionAtDistance(distance);
		info.normal = flip ? Vector(0.0f, 0.0f, -1.0f) : Vector(0.0f, 0.0f, 1.0f);
		info.material = material;
		return true;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		box = AABB(Vector(x0, y0, z - 0.0001f), Vector(x1, y1, z + 0.0001f));
		return true;
	}
};

class XZRectangle : public Hittable {
private:
	float x0, x1, z0, z1, y;
	Material *material;
	bool flip;
public:
	XZRectangle(float x0, float x1, float z0, float z1, float y, Material *material, bool flip = false) {
		this->x0 = x0;
		this->x1 = x1;
		this->z0 = z0;
		this->z1 = z1;
		this->y = y;
		this->material = material;
		this->flip = flip;
	}
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		float distance = (y - ray.origin().y()) / ray.direction().y();
		if (distance < minDistance || distance > maxDistance)
			return false;
		float x = ray.origin().x() + distance * ray.direction().x();
		float z = ray.origin().z() + distance * ray.direction().z();
		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;
		info.u = (x - x0) / (x1 - x0);
		info.v = (z - z0) / (z1 - z0);
		info.distance = distance;
		info.position = ray.positionAtDistance(distance);
		info.normal = flip ? Vector(0.0f, -1.0f, 0.0f) : Vector(0.0f, 1.0f, 0.0f);
		info.material = material;
		return true;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		box = AABB(Vector(x0, y - 0.0001f, z0), Vector(x1, y + 0.0001f, z1));
		return true;
	}
};

class YZRectangle : public Hittable {
private:
	float y0, y1, z0, z1, x;
	Material *material;
	bool flip;
public:
	YZRectangle(float y0, float y1, float z0, float z1, float x, Material *material, bool flip = false) {
		this->y0 = y0;
		this->y1 = y1;
		this->z0 = z0;
		this->z1 = z1;
		this->x = x;
		this->material = material;
		this->flip = flip;
	}
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		float distance = (x - ray.origin().x()) / ray.direction().x();
		if (distance < minDistance || distance > maxDistance)
			return false;
		float y = ray.origin().y() + distance * ray.direction().y();
		float z = ray.origin().z() + distance * ray.direction().z();
		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;
		info.u = (y - y0) / (y1 - y0);
		info.v = (z - z0) / (z1 - z0);
		info.distance = distance;
		info.position = ray.positionAtDistance(distance);
		info.normal = flip ? Vector(-1.0f, 0.0f, 0.0f) : Vector(1.0f, 0.0f, 0.0f);
		info.material = material;
		return true;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		box = AABB(Vector(x - 0.0001f, y0, z0), Vector(x + 0.0001f, y0, z0));
		return true;
	}
};