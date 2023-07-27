#pragma once

#include "Ray.h"

class Camera {
public:
	Camera() {
		Position = vec3(0, 0, 0);
		LeftDownPoint = vec3(-2.0, -1.0, -1.0);
		Horizontal = vec3(4.0, 0, 0);
		Vertical = vec3(0, 2.0, 0);
	}

	Ray GetRay(float u, float v) {
		return Ray(Position, LeftDownPoint + u * Horizontal + v * Vertical);
	}

public:
	vec3 Position;
	vec3 LeftDownPoint;
	vec3 Horizontal;
	vec3 Vertical;
};