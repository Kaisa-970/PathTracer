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

	vec3 operator+(const vec3& v2) const {
		return vec3(e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
	}
	vec3 operator-(const vec3& v2) const {
		return vec3(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
	}
	vec3 operator*(const vec3& v2) const {
		return vec3(e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]);
	}
	vec3 operator*(const float t) const {
		return vec3(e[0] * t, e[1] * t, e[2] * t);
	}

	vec3 operator/(const vec3& v2) const {
		return vec3(e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]);
	}
	vec3 operator/(const float t) const {
		return vec3(e[0] / t, e[1] / t, e[2] / t);
	}

	float dot(const vec3& v2) const {
		return e[0] * v2.e[0] + e[1] * v2.e[1] + e[2] * v2.e[2];
	}

	vec3 cross(const vec3& v2) const {
		return vec3(
			e[1] * v2.e[2] - e[2] * v2.e[1],
			e[2] * v2.e[0] - e[0] * v2.e[2],
			e[0] * v2.e[1] - e[1] * v2.e[0]
		);
	}

	vec3 unit_vector() const {

		return vec3(e[0],e[1],e[2]) / length();
	}

	vec3& operator+=(const vec3& v2) { 
		e[0] += v2.e[0]; 
		e[1] += v2.e[1];
		e[2] += v2.e[2];
		return *this;
	}
	vec3& operator-=(const vec3& v2) {
		e[0] -= v2.e[0];
		e[1] -= v2.e[1];
		e[2] -= v2.e[2];
		return *this;
	}
	vec3& operator*=(const vec3& v2) {
		e[0] *= v2.e[0];
		e[1] *= v2.e[1];
		e[2] *= v2.e[2];
		return *this;
	}
	vec3& operator/=(const vec3& v2) {
		e[0] /= v2.e[0];
		e[1] /= v2.e[1];
		e[2] /= v2.e[2];
		return *this;
	}
	vec3& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const float t) {
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	float length()const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	float squared_length()const {return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];}

	void make_unit_vector() {
		float l = length();
		e[0] /= l;
		e[1] /= l;
		e[2] /= l;
	}

private:
	friend std::ostream& operator<<(std::ostream& os,const vec3& v) {
		os << "(" << v.x()<<","
			<< v.y() << ","
			<< v.z() << ")";
		return os;
	}
public:
	float e[3];
};

vec3 operator*(const float t, const vec3& v) {
	return v * t;
}