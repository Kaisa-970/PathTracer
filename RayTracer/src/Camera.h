#pragma once

#include "Ray.h"
#include "Random.h"

#define M_PI    3.14159265
#define DEG2RAD 0.01745329
#define RAD2DEG 57.2957795

class Camera {
public:
	Camera() {
		Position = vec3(0.0, 0.0, 0.0);
		LeftDownPoint = vec3(-2.0, -1.0, -1.0);
		Horizontal = vec3(4.0, 0.0, 0.0);
		Vertical = vec3(0.0, 2.0, 0.0);
	}

	Camera(float vfov, float aspect) {
		float theta = vfov * DEG2RAD;
		float half_height = tan(theta / 2);
		float half_width = half_height * aspect;

		LeftDownPoint = vec3(-half_width, -half_height, -1.0);
		Horizontal = vec3(2.0 * half_width, 0.0, 0.0);
		Vertical = vec3(0.0, 2.0 * half_height, 0.0);
		Position = vec3(0.0, 0.0, 0.0);
	}

	Camera(vec3 lookfrom,vec3 lookat,vec3 vup,float vfov, float aspect) {
		float theta = vfov * DEG2RAD;
		float half_height = tan(theta / 2);
		float half_width = half_height * aspect;

		Position = lookfrom;
		vec3 w = (lookfrom - lookat).unit_vector();
		vec3 u = vup.cross(w).unit_vector();
		vec3 v = w.cross(u);

		LeftDownPoint = Position - w - half_width * u - half_height * v;
		Horizontal = 2.0 * half_width * u;
		Vertical = 2.0 * half_height * v;
	}

	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
		float aperture,float focus_dist) {
		Lens_Radius = aperture / 2.0;
		float theta = vfov * DEG2RAD;
		float half_height = tan(theta / 2);
		float half_width = half_height * aspect;

		Position = lookfrom;
		W = (lookfrom - lookat).unit_vector();
		U = vup.cross(W).unit_vector();
		V = W.cross(U);

		LeftDownPoint = Position - W * focus_dist - half_width * U * focus_dist- half_height * V * focus_dist;
		Horizontal = 2.0 * half_width * U * focus_dist;
		Vertical = 2.0 * half_height * V * focus_dist;
	}

	Ray GetRay(float u, float v) {
		vec3 rd = Lens_Radius * RandomPointInUnitCircle();
		vec3 offset = U * rd.x() + V * rd.y();
		return Ray(Position + offset, LeftDownPoint - Position - offset + u * Horizontal + v * Vertical);
	}

public:
	vec3 Position;
	vec3 LeftDownPoint;
	vec3 Horizontal;
	vec3 Vertical;
	vec3 U, V, W;
	float Lens_Radius;
};