#pragma once
#include "Ray.h"
#include "AABB.h"

class Material;

struct hit_record {
	float t;
	Vector3 p;
	Vector3 normal;
	Material *mat_ptr;
};

class Hitable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
};
