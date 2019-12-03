#pragma once
#include <float.h>
#include "AABB.h"
class Material;

const float PI = 3.14159265358979323846f;

struct HitInfo {
	float u;
	float v;
	float distance;
	Vector position;
	Vector normal;
	Material *material;

	HitInfo() {
		u = 0.0f;
		v = 0.0f;
		distance = 0.0f;
		position = Vector(0.0f, 0.0f, 0.0f);
		normal = Vector(0.0f, 0.0f, 0.0f);
		material = nullptr;
	}
};

class Hittable {
public:
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const = 0;
	virtual bool getBoundingBox(AABB &box) const = 0;
};

class HittableList : public Hittable {
private:
	Hittable **list;
	int size;
public:
	HittableList(Hittable **list, int size) {
		this->list = list;
		this->size = size;
	}
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		bool hitObjects = false;
		float tempDistance = maxDistance;
		HitInfo tempInfo;
		for (int i = 0; i < size; i++) {
			if (list[i]->hit(ray, minDistance, tempDistance, tempInfo)) {
				hitObjects = true;
				tempDistance = tempInfo.distance;
				info = tempInfo; // update to closest hit info
			}
		}
		return hitObjects;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		if (size < 1)
			return false;
		AABB tempBox;
		bool hasFirstBox = list[0]->getBoundingBox(tempBox);
		if (!hasFirstBox)
			return false;
		else
			box = tempBox;
		for (int i = 1; i < size; i++) {
			if (list[i]->getBoundingBox(tempBox))
				box = enclosing(box, tempBox);
			else
				return false;
		}
		return true;
	}
};