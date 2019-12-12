#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray() {}
	Ray(const Vector3 &a, const Vector3 &b, float ti = 0.0) { A = a; B = b; _time = ti; }
	Vector3 origin() const { return A; } // Returns a point.
	Vector3 direction() const { return B; } // Returns a vector.
	float time() const { return _time; }
	Vector3 point_at_parameter(float t) const { return A + t * B; } // Returns a point on the ray.
	
	Vector3 A; // Origin of ray (point).
	Vector3 B; // Direction of ray (vector).
	float _time;
};