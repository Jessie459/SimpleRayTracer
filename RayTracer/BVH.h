#pragma once
#include "Hittable.h"
#include "Random.h"

int myCompareX(const void *a, const void *b) {
	AABB boxL, boxR;
	Hittable *ha = *(Hittable **)a;
	Hittable *hb = *(Hittable **)b;
	if (!ha->getBoundingBox(boxL) || !hb->getBoundingBox(boxR)) {
		std::cerr << "no bounding box in BVH node constructor\n";
	}
	if (boxL.min().x() < boxR.min().x())
		return -1;
	else
		return 1;
}

int myCompareY(const void *a, const void *b) {
	AABB boxL, boxR;
	Hittable *ha = *(Hittable **)a;
	Hittable *hb = *(Hittable **)b;
	if (!ha->getBoundingBox(boxL) || !hb->getBoundingBox(boxR)) {
		std::cerr << "no bounding box in BVH node constructor\n";
	}
	if (boxL.min().y() < boxR.min().y())
		return -1;
	else
		return 1;
}

int myCompareZ(const void *a, const void *b) {
	AABB boxL, boxR;
	Hittable *ha = *(Hittable **)a;
	Hittable *hb = *(Hittable **)b;
	if (!ha->getBoundingBox(boxL) || !hb->getBoundingBox(boxR)) {
		std::cerr << "no bounding box in BVH node constructor\n";
	}
	if (boxL.min().z() < boxR.min().z())
		return -1;
	else
		return 1;
}

class BVHNode : public Hittable {
private:
	Hittable *childL;
	Hittable *childR;
	AABB box;
public:
	BVHNode() {}
	BVHNode(Hittable **list, int n) {
		int axis = int(3 * getRandom());
		if (axis == 0)
			qsort(list, n, sizeof(Hittable *), myCompareX);
		else if (axis == 1)
			qsort(list, n, sizeof(Hittable *), myCompareY);
		else
			qsort(list, n, sizeof(Hittable *), myCompareZ);

		if (n == 1) {
			childL = list[0];
			childR = list[0];
		}
		else if (n == 2) {
			childL = list[0];
			childR = list[1];
		}
		else {
			childL = new BVHNode(list, n / 2);
			childR = new BVHNode(list + n / 2, n - n / 2);
		}

		AABB boxL, boxR;
		if (!childL->getBoundingBox(boxL) || !childR->getBoundingBox(boxR)) {
			std::cerr << "no bounding box in BVH node constructor\n";
		}
		box = enclosing(boxL, boxR);
	}

	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		if (box.hit(ray, minDistance, maxDistance)) {
			HitInfo infoL, infoR;
			bool hitL = childL->hit(ray, minDistance, maxDistance, infoL);
			bool hitR = childR->hit(ray, minDistance, maxDistance, infoR);
			if (hitL && hitR) {
				if (infoL.distance <= infoR.distance)
					info = infoL;
				else
					info = infoR;
				return true;
			}
			else if (hitL) {
				info = infoL;
				return true;
			}
			else if (hitR) {
				info = infoR;
				return true;
			}
			else
				return false;
		}
	}

	virtual bool getBoundingBox(AABB &box) const override {
		box = this->box;
	}
};