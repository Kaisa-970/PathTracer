#pragma once

#include "Ray.h"

class Material;

struct HitResult
{
	float t;
	vec3 p;
	vec3 normal;
	Material* material;
};

class Hitable {
public:
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitResult& res) const = 0;
};

class HitableList : public Hitable 
{
public:
	HitableList() {}
	HitableList(Hitable** l, int n) :List(l), Size(n) {}
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitResult& res) const override;
public:
	Hitable** List;
	int Size;
};

bool HitableList::Hit(const Ray& ray, float t_min, float t_max, HitResult& res) const
{
	HitResult tempRes;
	bool hitAnthing = false;
	double closest_so_far = t_max;

	for (int i = 0; i < Size; i++) {
		if (List[i]->Hit(ray,t_min,closest_so_far,tempRes))
		{
			hitAnthing = true;
			closest_so_far = tempRes.t;
			res = tempRes;
		}
	}
	return hitAnthing;
}