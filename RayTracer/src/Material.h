#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Random.h"

class Material {
public:
	virtual bool Scatter(const Ray& r_in,const HitResult& hr,vec3& attenuation,Ray& Scattered)const = 0;
};

class Lambertian :public Material {
public:
	Lambertian(const vec3& a) :Albedo(a) {}

	virtual bool Scatter(const Ray& r_in, const HitResult& hr, vec3& attenuation, Ray& Scattered) const override {
		vec3 target = hr.p + hr.normal + RandomPointInUnitSphere();
		Scattered = Ray(hr.p, target - hr.p);
		attenuation = Albedo;
		return true;
	}

public:
	vec3 Albedo;

};

vec3 Reflect(const vec3& v, const vec3& n) {
	return v - 2 * v.dot(n) * n;
}

class Metal :public Material {
public:
	Metal(const vec3& a,float fz = 0.3f) :Albedo(a) {
		Fuzz = fz < 1 ? fz : 1;
	}

	virtual bool Scatter(const Ray& r_in, const HitResult& hr, vec3& attenuation, Ray& Scattered) const override {
		vec3 target = hr.p + hr.normal + RandomPointInUnitSphere();
		vec3 reflected = Reflect(r_in.direction.unit_vector(), hr.normal);
		Scattered = Ray(hr.p, reflected + Fuzz * RandomPointInUnitSphere());
		attenuation = Albedo;
		return Scattered.direction.dot(hr.normal) > 0;
	}

public:
	vec3 Albedo;
	float Fuzz;
};