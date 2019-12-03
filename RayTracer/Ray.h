#pragma once
#include "Vector.h"

class Ray {
private:
	Vector m_origin;
	Vector m_direction;

public:
	Ray() {}
	Ray(const Vector &origin, const Vector &direction) {
		m_origin = origin;
		m_direction = normalize(direction);
	}
	Vector origin() const {
		return m_origin;
	}
	Vector direction() const {
		return m_direction;
	}
	Vector positionAtDistance(float distance) const {
		return m_origin + distance * m_direction;
	}
};