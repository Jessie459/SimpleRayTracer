#pragma once
#include "Ray.h"

inline float getMin(float a, float b) { return a < b ? a : b; }
inline float getMax(float a, float b) { return a > b ? a : b; }

class AABB {
private:
	Vector m_min;
	Vector m_max;

public:
	AABB() {}
	AABB(const Vector &min, const Vector &max) :m_min(min), m_max(max) {}

	Vector min() const { return m_min; }
	Vector max() const { return m_max; }

	bool hit(const Ray &ray, float minDistance, float maxDistance) const {
		for (int i = 0; i < 3; i++) {
			float distance1 = (m_min[i] - ray.origin()[i]) / ray.direction()[i];
			float distance2 = (m_max[i] - ray.origin()[i]) / ray.direction()[i];
			float minDist = getMin(distance1, distance2);
			float maxDist = getMax(distance1, distance2);
			minDist = getMax(minDist, minDistance);
			maxDist = getMin(maxDist, maxDistance);
			if (maxDist <= minDist)
				return false;
		}
		return true;
	}
};

AABB enclosing(const AABB &box1, const AABB &box2) {
	Vector small(getMin(box1.min().x(), box2.min().x()),
				 getMin(box1.min().y(), box2.min().y()),
				 getMin(box1.min().z(), box2.min().z()));
	Vector   big(getMax(box1.max().x(), box2.max().x()),
				 getMax(box1.max().y(), box2.max().y()),
				 getMax(box1.max().z(), box2.max().z()));
	return AABB(small, big);
}