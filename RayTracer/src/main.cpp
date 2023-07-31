#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Sphere.h"
#include "Random.h"
#include "Material.h"
#include <mutex>

const float MAXFLOAT = std::numeric_limits<float>::max();
const int nx = 512;
const int ny = 256;
const int totalPixels = nx * ny;

std::atomic<int> renderedPixels(0);

const int ns = 100;
const int num_threads = 14;

vec3 getColor(const Ray& ray,Hitable* scene,int depth) {
	
	HitResult hr;
	if (scene->Hit(ray, 0.001, MAXFLOAT, hr))
	{
		// vec3 target = hr.p + hr.normal + RandomPointInUnitSphere();
		Ray scattered;
		vec3 attenuation;
		if (depth<50 && 
			hr.material->Scatter(ray, hr, attenuation, scattered))
		{
			return attenuation * getColor(scattered, scene, depth + 1);
		}
		else 
		{
			return vec3(0.0,0.0,0.0);
		}
	}
	vec3 unit_dir = ray.direction.unit_vector();
	float t = 0.5 * (unit_dir.y() + 1.0f);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

float renderProgress() {
	const int barWidth = 70;
	float progress = static_cast<float>(renderedPixels) / totalPixels;
	int pos = static_cast<int>(barWidth * progress);

	std::cout << "[";
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0) << "%\r";
	std::cout.flush();
	return progress;
}

void renderPixels(int thread_id, Hitable* scene, Camera& cam,
	vec3** img, std::mutex& mtx) {
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = thread_id; i < nx; i+=num_threads)
		{
			vec3 col(0.0, 0.0, 0.0);
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

			renderedPixels.fetch_add(1);
		}
	}
}

void simulateRender() {
	while (renderProgress()<1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::cout << std::endl;
}


int main() {


	// vec3 img[nx][ny];

	vec3** img = new vec3 * [nx];
	for (int i = 0; i < nx; ++i) {
		img[i] = new vec3[ny];
	}

	std::ofstream fs("output.ppm");

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	fs<< "P3\n" << nx << " " << ny << "\n255\n";
	
	Camera cam(vec3(-2,2,1),vec3(0,0,-1),vec3(0,1,0), 90, float(nx) / float(ny));
	float R = cos(M_PI / 4.0);

	Hitable* objList[4];
	objList[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian({0.1,0.2,0.5}));
	// objList[0] = new Sphere(vec3(0, 0, -1), 0.5, new Dielectric(1.5));
	objList[1] = new Sphere(vec3(0, -100.5, -1), 100.0, new Lambertian({ 0.8,0.8,0.0 }));
	objList[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal({ 0.8,0.6,0.2 }, 0.1));
	objList[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	// objList[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5));

	Hitable* scene = new HitableList(objList, 4);

	std::thread threads[num_threads];
	std::mutex mtx;

	std::thread progressThread(simulateRender);

	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < num_threads; i++)
	{
		threads[i] = std::thread(renderPixels, i, scene, std::ref(cam),
			img, std::ref(mtx));
	}
	
	for (int i = 0; i < num_threads; i++)
	{
		threads[i].join();
	}

	progressThread.join();

	auto endTime = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
		(endTime - startTime);

	std::cout << "Render time: " << duration.count() / 1000.0 << " seconds\n";


	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			fs << int(img[i][j].r()) << " " << int(img[i][j].g()) << " " << int(img[i][j].b()) << "\n";
		}
	}

	fs.close();

	delete[] img;
}