#pragma once

#include "Hitable.h"

class Sphere : public Hitable
{
public:
	Sphere() = default;
	Sphere(const vec3& c,float r):center(c),radius(r){}

	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitResult& res) const override
	{
		vec3 oc = ray.origin - center;
		float a = ray.direction.squared_length();
		float b = oc.dot(ray.direction);
		float c = oc.squared_length() - radius * radius;

		float discriminant = b * b - a * c;

		if (discriminant < 0)
		{
			return false;
		}
		else {
			float temp = (-b - sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min)
			{
				res.t = temp;
				res.p = ray.PointAtDistance(res.t);
				res.normal = (res.p - center) / radius;
				return true;
			}

			temp = (-b + sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min)
			{
				res.t = temp;
				res.p = ray.PointAtDistance(res.t);
				res.normal = (res.p - center) / radius;
				return true;
			}
		}

		return false;

	}



public:
	vec3 center;
	float radius;

};