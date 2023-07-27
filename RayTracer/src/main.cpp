#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Sphere.h"
#include "Random.h"

const float MAXFLOAT = 100000;

vec3 getColor(const Ray& ray,Hitable* scene) {
	
	HitResult hr;
	if (scene->Hit(ray, 0, MAXFLOAT, hr))
	{
		vec3 target = hr.p + hr.normal + RandomPointInUnitSphere();
		return 0.5 * getColor(Ray(hr.p,target - hr.p),scene);
	}
	vec3 unit_dir = ray.direction.unit_vector();
	float t = 0.5 * (unit_dir.y() + 1.0f);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}


int main() {
	int nx = 600;
	int ny = 300;
	int ns = 4;

	std::ofstream fs("output.ppm");

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	fs<< "P3\n" << nx << " " << ny << "\n255\n";
	
	Camera cam;

	Hitable* objList[2];
	objList[0] = new Sphere(vec3(0, 0, -1), 0.5);
	objList[1] = new Sphere(vec3(0, -100.5, -1), 100);

	Hitable* scene = new HitableList(objList, 2);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(i + GetRandom01()) / float(nx);
				float v = float(j + GetRandom01()) / float(ny);
				Ray ray = cam.GetRay(u, v);
				col += getColor(ray, scene);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));// gamma½ÃÕý,gamma = 2
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			fs<< ir << " " << ig << " " << ib << "\n";
		}
	}

	fs.close();
}