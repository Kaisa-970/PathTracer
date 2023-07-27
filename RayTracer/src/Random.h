#pragma once

#include <random>

float GetRandom01() {
	std::random_device rd;
	std::mt19937 gen(rd());

	float min_value = 0;
	float max_value = 1;
	std::uniform_real_distribution<float> distribution(min_value, max_value);

	return distribution(gen);
}

vec3 RandomPointInUnitSphere() {
	vec3 p;

	do {
		p = 2.0 * vec3(GetRandom01(), GetRandom01(), GetRandom01()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0f);
	return p;
}