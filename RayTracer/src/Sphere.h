#pragma once

#include "Ray.h"

class Sphere {
public:
	Sphere() = default;
	Sphere(const vec3& c,float r):center(c),radius(r){}

	float Hit(const Ray& ray) {
		vec3 oc = ray.origin - center;
		float a = ray.direction.squared_length();
		float b = 2.0 * oc.dot(ray.direction);
		float c = oc.squared_length() - radius * radius;

		float discriminant = b * b - 4 * a * c;

		if (discriminant<0)
		{
			return -1.0;
		}
		else {
			return (-b - sqrt(discriminant)) / (2.0 * a);
		}
	}

public:
	vec3 center;
	float radius;
};