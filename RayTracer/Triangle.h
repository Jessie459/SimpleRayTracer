#pragma once
#include <algorithm>
#include "Hittable.h"

class Triangle : public Hittable {
private:
	Vector v1, v2, v3;
	Vector n1, n2, n3;
	bool useThreeNormals;
	Material* material;

public:
	Triangle(const Vector& v1, const Vector& v2, const Vector& v3, Material *material) {
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->useThreeNormals = false;
		this->material = material;

	}

	Triangle(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& n1, const Vector& n2, const Vector& n3, Material* material) {
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->n1 = n1;
		this->n2 = n2;
		this->n3 = n3;
		this->useThreeNormals = true;
		this->material = material;
	}

	virtual bool hit(const Ray& ray, float minDistance, float maxDistance, HitInfo& info) const override {
		Vector edge1 = v2 - v1;
		Vector edge2 = v3 - v1;
		Vector t = ray.origin() - v1;
		Vector q = cross(t, edge1);
		Vector p = cross(ray.direction(), edge2);

		float det = dot(p, edge1);
		if (fabs(det) < 1e-5f)
			return false;
		float invDet = 1.0f / det;

		float u = dot(p, t) * invDet;
		if (u < 0.0f || u > 1.0f)
			return false;
		float v = dot(q, ray.direction()) * invDet;
		if (v < 0.0f || u + v > 1.0f)
			return false;
		float distance = dot(q, edge2) * invDet;
		if (distance < minDistance || distance > maxDistance)
			return false;

		info.u = u;
		info.v = v;
		info.distance = distance;
		info.position = ray.positionAtDistance(distance);
		info.material = material;
		if (useThreeNormals) {
			float w = 1.0f - u - v;
			info.normal = (w * n1 + u * n2 + v * n3).normalized();
		}
		else {
			info.normal = cross(edge1, edge2).normalized();
		}
		return true;
	}

	virtual bool getBoundingBox(AABB& box) const override {
		Vector boxMin(std::min(std::min(v1.x(), v2.x()), v3.x()), std::min(std::min(v1.y(), v2.y()), v3.y()), std::min(std::min(v1.z(), v2.z()), v3.z()));
		Vector boxMax(std::max(std::max(v1.x(), v2.x()), v3.x()), std::max(std::max(v1.y(), v2.y()), v3.y()), std::max(std::max(v1.z(), v2.z()), v3.z()));
		Vector diff = boxMax - boxMin;
		if (diff.x() < 1e-5f)
			boxMax.x() += 1e-5f;
		if (diff.y() < 1e-5f)
			boxMax.y() += 1e-5f;
		if (diff.z() < 1e-5f)
			boxMax.z() += 1e-5f;
		box = AABB(boxMin, boxMax);
		return true;
	}
};