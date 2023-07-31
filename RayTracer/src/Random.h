#pragma once

#include <random>

float GetRandom01() {
	std::random_device rd;
	std::ranlux48 gen(rd());
	// std::mt19937_64 gen(rd());

	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	return distribution(gen);
}

vec3 RandomPointInUnitSphere() {
	vec3 p;

	do {
		p = 2.0 * vec3(GetRandom01(), GetRandom01(), GetRandom01()) - vec3(1.0, 1.0, 1.0);
	} while (p.squared_length() >= 1.0f);
	return p;
}