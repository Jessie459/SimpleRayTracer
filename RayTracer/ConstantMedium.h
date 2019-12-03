#pragma once
#include "Hittable.h"
#include "Texture.h"
#include "Material.h"

class ConstantMedium : public Hittable {
private:
	Hittable *boundary;
	float density;
	Material *material;

public:
	ConstantMedium(Hittable *boundary, float density, Texture *texture) {
		this->boundary = boundary;
		this->density = density;
		this->material = new Isotropic(texture);
	}

	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		HitInfo info1, info2;
		if (boundary->hit(ray, -FLT_MAX, FLT_MAX, info1)) {
			if (boundary->hit(ray, info1.distance + 0.0001f, FLT_MAX, info2)) {
				if (info1.distance < minDistance)
					info1.distance = minDistance;
				if (info2.distance > maxDistance)
					info2.distance = maxDistance;
				if (info1.distance >= info2.distance)
					return false;
				if (info1.distance < 0)
					info1.distance = 0;

				float insideDistance = info2.distance - info1.distance;
				float randomDistance = -log(getRandom()) / density;
				if (randomDistance < insideDistance) {
					info.distance = info1.distance + randomDistance;
					info.position = ray.positionAtDistance(info.distance);
					info.material = material;
					return true;
				}
			}
		}
		return false;
	}

	virtual bool getBoundingBox(AABB &box) const override {
		return boundary->getBoundingBox(box);
	}
};