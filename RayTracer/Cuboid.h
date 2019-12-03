#pragma once
#include "Hittable.h"
#include "Rectangle.h"

class Cuboid : public Hittable {
private:
	Hittable *hittableList;
	Vector minPosition, maxPosition;

public:
	Cuboid(const Vector &minPosition, const Vector &maxPosition, Material *material) {
		this->minPosition = minPosition;
		this->maxPosition = maxPosition;

		Hittable **list = new Hittable *[6];
		list[0] = new XYRectangle(minPosition.x(), maxPosition.x(), minPosition.y(), maxPosition.y(), maxPosition.z(), material, false);
		list[1] = new XYRectangle(minPosition.x(), maxPosition.x(), minPosition.y(), maxPosition.y(), minPosition.z(), material, true);
		list[2] = new XZRectangle(minPosition.x(), maxPosition.x(), minPosition.z(), maxPosition.z(), maxPosition.y(), material, false);
		list[3] = new XZRectangle(minPosition.x(), maxPosition.x(), minPosition.z(), maxPosition.z(), minPosition.y(), material, true);
		list[4] = new YZRectangle(minPosition.y(), maxPosition.y(), minPosition.z(), maxPosition.z(), maxPosition.x(), material, false);
		list[5] = new YZRectangle(minPosition.y(), maxPosition.y(), minPosition.z(), maxPosition.z(), minPosition.x(), material, true);
		hittableList =  new HittableList(list, 6);
	}

	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		return hittableList->hit(ray, minDistance, maxDistance, info);
	}

	virtual bool getBoundingBox(AABB &box) const override {
		box = AABB(minPosition, maxPosition);
		return true;
	}
};