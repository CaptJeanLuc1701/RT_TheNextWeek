#pragma once
#include "Ray.h"
#include <memory>

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
public:
	AABB() {}
	AABB(const Vector3& a, const Vector3& b) { _min = a; _max = b; }
	Vector3 min() { return _min; }
	Vector3 max() { return _max; }
	bool hit(const Ray& r, float tmin, float tmax) const;
	Vector3 _min, _max;
};

inline bool AABB::hit(const Ray& r, float tmin, float tmax) const {
	for (int a = 0; a < 3; a++) {
		float invD = 1.0f / r.direction()[a];
		float t0 = (_min[a] - r.origin()[a]) * invD;
		float t1 = (_max[a] - r.origin()[a]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)
			return false;
	}
	return true;
}

AABB surrounding_box(AABB box0, AABB box1) {
	Vector3 small(ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));
	Vector3 big(ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));
	return AABB(small, big);
}
