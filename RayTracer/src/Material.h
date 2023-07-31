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
	// vec3 nv = v.unit_vector();
	return v - 2 * v.dot(n) * n;
}

bool Refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = v.unit_vector();
	float dt = uv.dot(n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant>0)
	{
		refracted = (ni_over_nt * (v - n * dt) - n * sqrt(discriminant)).unit_vector();
		return true;
	}
	else {
		return false;
	}
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;

	return r0 + (1 - r0) * pow((1 - cosine), 5);
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

class Dielectric :public Material
{
public:
	Dielectric(float ri) :Ref_idx(ri) {}

	virtual bool Scatter(const Ray& r_in, const HitResult& hr, vec3& attenuation, Ray& Scattered) const override {

		vec3 outward_normal;
		vec3 reflected = Reflect(r_in.direction, hr.normal);

		attenuation = vec3(1.0, 1.0, 1.0);

		float ni_over_nt;
		vec3 refracted;

		float reflect_prob;
		float cosine;
		if (r_in.direction.dot(hr.normal) > 0)
		{
			outward_normal = -hr.normal;
			ni_over_nt = Ref_idx;

			cosine = Ref_idx * r_in.direction.dot(hr.normal) / r_in.direction.length();
			//if (cosine>1.0f)
			//{
			//	Scattered = Ray(hr.p, reflected);

			//	return true;
			//}
			//cosine = cosine > 1.0 ? 1.0:cosine;
		}
		else {
			outward_normal = hr.normal;
			ni_over_nt = 1.0 / Ref_idx;

			cosine = -r_in.direction.dot(hr.normal) / r_in.direction.length();

		}

		if (Refract(r_in.direction,outward_normal,ni_over_nt,refracted))
		{
			reflect_prob = schlick(cosine, Ref_idx);
			// Scattered = Ray(hr.p, refracted);
		}
		else {
			// reflected = vec3(0, 1, 0);
			Scattered = Ray(hr.p, reflected);
			reflect_prob = 1.0;

			// return false;
			return true;
		}

		if (GetRandom01() < reflect_prob) {
			Scattered = Ray(hr.p, reflected);
		}
		else {
			Scattered = Ray(hr.p, refracted);
		}
		return true;
	}

public:
	float Ref_idx;
};

//class dielectric : public Material {
//public:
//	dielectric(double index_of_refraction) : ir(index_of_refraction) {}
//
//	virtual bool Scatter(
//		const Ray& r_in, const HitResult& rec, vec3& attenuation, Ray& scattered
//	) const override {
//		attenuation = vec3(1.0, 1.0, 1.0);
//		double refraction_ratio = r_in.direction.dot(rec.normal) < 0 ? (1.0 / ir) : ir;
//
//		vec3 unit_direction = r_in.direction.unit_vector();
//		double cos_theta = fmin(-unit_direction.dot(rec.normal)), 1.0);
//		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
//
//		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
//		vec3 direction;
//		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
//			direction = reflect(unit_direction, rec.normal);
//		else
//			direction = refract(unit_direction, rec.normal, refraction_ratio);
//
//		scattered = ray(rec.p, direction);
//		return true;
//	}
//
//public:
//	double ir; // Index of Refraction
//
//private:
//	static double reflectance(double cosine, double ref_idx) {
//		// Use Schlick's approximation for reflectance.
//		auto r0 = (1 - ref_idx) / (1 + ref_idx);
//		r0 = r0 * r0;
//		return r0 + (1 - r0) * pow((1 - cosine), 5);
//	}
//};