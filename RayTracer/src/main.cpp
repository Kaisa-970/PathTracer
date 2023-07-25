#include <iostream>
#include "Sphere.h"
#include <fstream>

vec3 getColor(const Ray& ray) {
	Sphere sphere({ 0,0,-1 }, 0.5);
	float t = sphere.Hit(ray);
	if (t>0)
	{
		vec3 N = (ray.PointAtDistance(t) - sphere.center).unit_vector();
		return 0.5 * (N + vec3(1, 1, 1));
	}
	vec3 unit_dir = ray.direction.unit_vector();
	t = 0.5 * (unit_dir.y() + 1.0f);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}


int main() {
	int nx = 600;
	int ny = 300;

	std::ofstream fs("output.ppm");

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	fs<< "P3\n" << nx << " " << ny << "\n255\n";
	
	vec3 leftDownPoint(-2.0, -1.0, -1.0);
	vec3 horizental(4.0, 0, 0);
	vec3 vertical(0, 2.0, 0);
	vec3 camPos(0, 0, 0);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			vec3 dir = leftDownPoint + u * horizental + v * vertical;
			Ray ray(camPos, dir);
			vec3 col = getColor(ray);
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			fs<< ir << " " << ig << " " << ib << "\n";
		}
	}

	fs.close();
}