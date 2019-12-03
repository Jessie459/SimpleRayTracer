#pragma once
#include <iostream>
#include <math.h>
#include <stdlib.h>

class Vector {
public:
	float e[3];

	Vector(float e0 = 0.0f, float e1 = 0.0f, float e2 = 0.0f) {
		this->e[0] = e0;
		this->e[1] = e1;
		this->e[2] = e2;
	}

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline float& x() { return e[0]; }
	inline float& y() { return e[1]; }
	inline float& z() { return e[2]; }
	inline float& r() { return e[0]; }
	inline float& g() { return e[1]; }
	inline float& b() { return e[2]; }

	inline const Vector &operator+() const { return *this; }
	inline Vector operator-() const { return Vector(-e[0], -e[1], -e[2]); }

	inline float operator[](int i) const { return e[i]; }
	inline float &operator[](int i) { return e[i]; }

	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float squaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

	inline Vector &operator+=(const Vector &v);
	inline Vector &operator-=(const Vector &v);
	inline Vector &operator*=(const Vector &v);
	inline Vector &operator/=(const Vector &v);

	inline Vector &operator*=(const float t);
	inline Vector &operator/=(const float t);

	inline Vector &normalized();
};

inline Vector &Vector::operator+=(const Vector &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vector &Vector::operator-=(const Vector &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vector &Vector::operator*=(const Vector &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vector &Vector::operator/=(const Vector &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vector &Vector::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vector &Vector::operator/=(const float t) {
	float k = 1.0f / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vector &Vector::normalized() {
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
	return *this;
}

inline Vector operator+(const Vector &v1, const Vector &v2) {
	return Vector(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector operator-(const Vector &v1, const Vector &v2) {
	return Vector(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector operator*(const Vector &v1, const Vector &v2) {
	return Vector(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vector operator/(const Vector &v1, const Vector &v2) {
	return Vector(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vector operator*(float t, const Vector &v) {
	return Vector(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector operator*(const Vector &v, float t) {
	return Vector(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector operator/(Vector v, float t) {
	return Vector(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const Vector &v1, const Vector &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vector cross(const Vector &v1, const Vector &v2) {
	return Vector(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
				  v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
				  v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vector normalize(const Vector &v) {
	return v / v.length();
}

inline std::istream &operator >> (std::istream &is, Vector &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}