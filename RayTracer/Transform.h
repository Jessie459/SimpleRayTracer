#pragma once
#include "Hittable.h"

class Translate : public Hittable {
private:
	Hittable *hittable;
	Vector offset;

public:
	Translate(Hittable *hittable, const Vector &offset) {
		this->hittable = hittable;
		this->offset = offset;
	}

	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		Ray translatedRay(ray.origin() - offset, ray.direction());
		if (hittable->hit(translatedRay, minDistance, maxDistance, info)) {
			info.position += offset;
			return true;
		}
		else
			return false;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		if (hittable->getBoundingBox(box)) {
			box = AABB(box.min() + offset, box.max() + offset);
			return true;
		}
		else
			return false;
	}
};

class RotateAroundY : public Hittable {
private:
	Hittable *hittable;
	float sinValue;
	float cosValue;
	bool hasBoundingBox;
	AABB box;

public:
	RotateAroundY(Hittable *hittable, float degree) {
		this->hittable = hittable;

		// compute sin value and cos value from given angle
		float angle = degree * PI / 180.0f;
		sinValue = sin(angle);
		cosValue = cos(angle);

		// get original bounding box
		hasBoundingBox = hittable->getBoundingBox(box);

		// rotate eight vertices of the bounding box respectively
		Vector min(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++) {
					float x = (1 - i) * box.min().x() + i * box.max().x();
					float y = (1 - j) * box.min().y() + j * box.max().y();
					float z = (1 - k) * box.min().z() + k * box.max().z();
					float rotatedX =  cosValue * x + sinValue * z;
					float rotatedZ = -sinValue * x + cosValue * z;
					Vector rotated(rotatedX, y, rotatedZ);
					for (int a = 0; a < 3; a++) {
						if (max[a] < rotated[a])
							max[a] = rotated[a];
						if (min[a] > rotated[a]) {
							min[a] = rotated[a];
						}
					}
				}

		// obtain new bounding box
		box = AABB(min, max);
	}

	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		Vector origin = ray.origin();
		Vector direction = ray.direction();
		origin[0] = cosValue * ray.origin()[0] - sinValue * ray.origin()[2];
		origin[2] = sinValue * ray.origin()[0] + cosValue * ray.origin()[2];
		direction[0] = cosValue * ray.direction()[0] - sinValue * ray.direction()[2];
		direction[2] = sinValue * ray.direction()[0] + cosValue * ray.direction()[2];
		Ray rotatedRay(origin, direction);
		if (hittable->hit(rotatedRay, minDistance, maxDistance, info)) {
			Vector position = info.position;
			Vector normal = info.normal;
			position[0] =  cosValue * info.position[0] + sinValue * info.position[2];
			position[2] = -sinValue * info.position[0] + cosValue * info.position[2];
			normal[0] =  cosValue * info.normal[0] + sinValue * info.normal[2];
			normal[2] = -sinValue * info.normal[0] + cosValue * info.normal[2];
			info.position = position;
			info.normal = normal;
			return true;
		}
		else
			return false;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		box = this->box;
		return hasBoundingBox;
	}
};