#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Sphere.h"
#include "Random.h"
#include "Material.h"
#include <mutex>

const float MAXFLOAT = 100000;
const int nx = 256;
const int ny = 128;
const int ns = 100;
const int num_threads = 14;

vec3 getColor(const Ray& ray,Hitable* scene,int depth) {
	
	HitResult hr;
	if (scene->Hit(ray, 0, MAXFLOAT, hr))
	{
		vec3 target = hr.p + hr.normal + RandomPointInUnitSphere();
		Ray scattered;
		vec3 attenuation;
		if (depth<50 && 
			hr.material->Scatter(ray, hr, attenuation, scattered))
		{
			return attenuation * getColor(scattered, scene, depth + 1);
		}
		else {
			return vec3(0,0,0);
		}
		
	}
	vec3 unit_dir = ray.direction.unit_vector();
	float t = 0.5 * (unit_dir.y() + 1.0f);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void renderPixels(int thread_id, Hitable* scene, Camera& cam,
	vec3(&img)[nx][ny], std::mutex& mtx) {
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = thread_id; i < nx; i+=num_threads)
		{
			vec3 col(0, 0, 0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(i + GetRandom01()) / float(nx);
				float v = float(j + GetRandom01()) / float(ny);
				Ray ray = cam.GetRay(u, v);
				col += getColor(ray, scene, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));// gamma½ÃÕý,gamma = 2
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			img[i][j] = vec3(ir, ig, ib);
		}
	}
}


int main() {


	vec3 img[nx][ny];

	std::ofstream fs("output.ppm");

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	fs<< "P3\n" << nx << " " << ny << "\n255\n";
	
	Camera cam;

	Hitable* objList[4];
	objList[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian({0.8,0.3,0.3}));
	objList[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian({ 0.8,0.3,0.0 }));
	objList[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal({ 0.8,0.6,0.2 }));
	objList[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Metal({ 0.8,0.8,0.8 }));

	Hitable* scene = new HitableList(objList, 4);

	std::thread threads[num_threads];
	std::mutex mtx;

	for (int i = 0; i < num_threads; i++)
	{
		threads[i] = std::thread(renderPixels, i, scene, std::ref(cam), 
			std::ref(img), std::ref(mtx));
	}
	
	for (int i = 0; i < num_threads; i++)
	{
		threads[i].join();
	}

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			fs << int(img[i][j].r()) << " " << int(img[i][j].g()) << " " << int(img[i][j].b()) << "\n";
		}
	}

	fs.close();
}