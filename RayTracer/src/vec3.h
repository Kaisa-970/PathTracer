#pragma once

#include <math.h>
#include <stdio.h>
#include <iostream>

class vec3 {
public:
	vec3() {}
	vec3(float x,float y,float z)
	{
		e[0] = x;
		e[1] = y;
		e[2] = z;
	}
	float x()const { return e[0]; }
	float y()const { return e[1]; }
	float z()const { return e[2]; }
	float r()const { return e[0]; }
	float g()const { return e[1]; }
	float b()const { return e[2]; }

	const vec3& operator+() const { return *this; }
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i)const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	inline vec3 operator+(const vec3& v2) {
		return vec3(e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
	}
	inline vec3 operator-(const vec3& v2) {
		return vec3(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
	}
	inline vec3 operator*(const vec3& v2) {
		return vec3(e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]);
	}
	inline vec3 operator/(const vec3& v2) {
		return vec3(e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]);
	}

	float dot(const vec3& v2) {
		return e[0] * v2.e[0] + e[1] * v2.e[1] + e[2] * v2.e[2];
	}

	vec3 cross(const vec3& v2) {
		return vec3(
			e[1] * v2.e[2] - e[2] * v2.e[1],
			e[2] * v2.e[0] - e[0] * v2.e[2],
			e[0] * v2.e[1] - e[1] * v2.e[0]
		);
	}

	// vec3 unit_vector() { return *this / length(); }

	vec3& operator+=(const vec3& v2);
	vec3& operator-=(const vec3& v2);
	vec3& operator*=(const vec3& v2);
	vec3& operator/=(const vec3& v2);
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

	float length()const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	float squared_length()const {return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];}

	void make_unit_vector();
public:
	float e[3];
};