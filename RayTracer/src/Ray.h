#pragma once
#include "vec3.h"

class Ray {
public:
	Ray(){}
	Ray(const vec3& ori,const vec3& dir):origin(ori),direction(dir){}

	vec3 PointAtDistance(float t)const { return origin + t * direction; }
public:
	vec3 origin;
	vec3 direction;
};