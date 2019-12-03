#pragma once
#include "Hittable.h"

class Sphere : public Hittable {
private:
	Vector center;
	float radius;
	Material *material;

	void getSphereUV(const Vector &p, float &u, float &v) const {
		float angle1 = atan2(p.z(), p.x());
		float angle2 = asin(p.y());
		u = 1.0f - (angle1 + PI) / (2.0f * PI);
		v = 1.0f - (angle2 + PI / 2.0f) / PI;
	}

public:
	Sphere(Vector center, float radius, Material *material) {
		this->center = center;
		this->radius = radius;
		this->material = material;
	}
	virtual bool hit(const Ray &ray, float minDistance, float maxDistance, HitInfo &info) const override {
		Vector v = ray.origin() - center;
		float a = dot(ray.direction(), ray.direction());
		float b = dot(v, ray.direction());
		float c = dot(v, v) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0) {
			float distance = (-b - sqrt(b * b - a * c)) / a;
			if (distance > minDistance && distance < maxDistance) {
				info.distance = distance;
				info.position = ray.positionAtDistance(distance);
				info.normal = (info.position - center) / radius;
				info.material = material;
				getSphereUV(info.normal, info.u, info.v);
				return true;
			}
			distance = (-b + sqrt(b * b - a * c)) / a;
			if (distance > minDistance && distance < maxDistance) {
				info.distance = distance;
				info.position = ray.positionAtDistance(distance);
				info.normal = (info.position - center) / radius;
				info.material = material;
				getSphereUV(info.normal, info.u, info.v);
				return true;
			}
		}
		return false;
	}
	virtual bool getBoundingBox(AABB &box) const override {
		box = AABB(center - Vector(radius, radius, radius), center + Vector(radius, radius, radius));
		return true;
	}
};