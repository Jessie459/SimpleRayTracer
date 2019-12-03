#pragma once
#include "Hittable.h"
#include "Texture.h"
#include "Random.h"

float schlick(float cosine, float n1, float n2) {
	float r0 = (n1 - n2) / (n1 + n2);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const Vector &incident, const Vector &normal, float ratio, Vector &refracted) {
	float cosine = dot(incident, normal);
	float discriminant = 1.0f - ratio * ratio * (1.0f - cosine * cosine);
	if (discriminant > 0) {
		refracted = ratio * (incident - cosine * normal) - sqrt(discriminant) * normal;
		return true;
	}
	else
		return false;
}

Vector reflect(const Vector &incident, const Vector &normal) {
	return incident - 2.0f * dot(incident, normal) * normal;
}

class Material {
public:
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const = 0;
	virtual Vector emit(float u, float v, const Vector &p) const {
		return Vector(0.0f, 0.0f, 0.0f);
	}
};

class Diffuse : public Material {
private:
	Texture *texture;
public:
	Diffuse(Texture *texture) {
		this->texture = texture;
	}
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const override {
		return false;
	}
	virtual Vector emit(float u, float v, const Vector &p) const override {
		return texture->value(u, v, p);
	}
};

class Isotropic : public Material {
private:
	Texture *texture;
public:
	Isotropic(Texture *texture) {
		this->texture = texture;
	}
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const override {
		scatteredRay = Ray(info.position, getRandomInSphere());
		attenuation = texture->value(info.u, info.v, info.position);
		return true;
	}
};

class Lambertian : public Material {
private:
	Texture *texture;
public:
	Lambertian(Texture *texture) {
		this->texture = texture;
	}
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const override {
		Vector target = info.position + info.normal + getRandomInSphere();
		scatteredRay = Ray(info.position, target - info.position);
		attenuation = texture->value(info.u, info.v, info.position);
		return true;
	}
};

class Metal : public Material {
private:
	Vector albedo;
	float fuzz;
public:
	Metal(const Vector &albedo, float fuzz = 1.0f) {
		this->albedo = albedo;
		if (fuzz < 1)
			this->fuzz = fuzz;
		else
			this->fuzz = 1.0f;
	}
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const override {
		Vector reflected = reflect(incidentRay.direction(), info.normal);
		scatteredRay = Ray(info.position, reflected + fuzz * getRandomInSphere());
		attenuation = albedo;
		return (dot(scatteredRay.direction(), info.normal) > 0);
	}
};

class Dielectric : public Material {
private:
	float n1, n2; // indices of refraction of the two media at the interface
public:
	Dielectric(float n1, float n2) {
		this->n1 = n1;
		this->n2 = n2;
	}
	virtual bool scatter(const Ray &incidentRay, const HitInfo &info, Vector &attenuation, Ray &scatteredRay) const override {
		// set attenuation
		attenuation = Vector(1.0f, 1.0f, 1.0f);

		// get reflected direction by calling reflect function
		Vector reflected = reflect(incidentRay.direction(), info.normal);

		Vector normal;
		float ratio;
		float cosine;
		if (dot(incidentRay.direction(), info.normal) > 0) {
			normal = -info.normal;
			ratio = n2 / n1;
			cosine = dot(incidentRay.direction(), info.normal);
		}
		else {
			normal = info.normal;
			ratio = n1 / n2;
			cosine = -dot(incidentRay.direction(), info.normal);
		}

		// get reflection coefficient by Schlick's approximation
		Vector refracted;
		float reflectionCoef;
		if (refract(incidentRay.direction(), normal, ratio, refracted))
			reflectionCoef = schlick(cosine, n1, n2);
		else
			reflectionCoef = 1.0f;

		if (getRandom() < reflectionCoef)
			scatteredRay = Ray(info.position, reflected);
		else
			scatteredRay = Ray(info.position, refracted);
		return true;
	}
};