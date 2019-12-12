#include <fstream>
#include "float.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Random.h"
#include "Material.h"

using namespace std;

Hitable *random_scene() {
	int n = 500;
	Hitable **list = new Hitable*[n + 1];
	list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(Vector3(0.2, 0.5, 0.3)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float choose_mat = random_double();
			Vector3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
			if ((center - Vector3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new MovingSphere(
						center,
						center + Vector3(0, 0.5*random_double(), 0),
						0.0, 1.0, 0.2,
						new Lambertian(
							Vector3(random_double()*random_double(),
								random_double()*random_double(),
								random_double()*random_double())
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(
						center, 0.2,
						new Metal(
							Vector3(0.5*(1 + random_double()),
								0.5*(1 + random_double()),
								0.5*(1 + random_double())),
							0.5*random_double()
						)
					);
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vector3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vector3(-4, 1, 0), 1.0, new Lambertian(Vector3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vector3(4, 1, 0), 1.0, new Metal(Vector3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
}

Vector3 color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vector3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vector3(0, 0, 0);
		}
	}
	else {
		Vector3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
	}
}

int main()
{
	// Set up output file.
	ofstream file;
	file.open("sample.ppm");
	int nx = 320; // width
	int ny = 240; // height
	file << "P3\n" << nx << " " << ny << "\n255\n"; // ppm header.

	// Set up objects.
	Hitable *world = random_scene();

	// Set up camera.
	Vector3 lookfrom(13, 2, 3);
	Vector3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	Camera cam(lookfrom, lookat, Vector3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);

	int ns = 10; // Number of antialiasing samples.

	for (int j = ny - 1; j >= 0; j--) { // Each row
		for (int i = 0; i < nx; i++) { // Each column
			Vector3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { // Each antialiasing sample
				float u = float(i + random_double()) / float(nx); // Horizontal factor of ray direction.
				float v = float(j + random_double()) / float(ny); // Vertical factor of ray direction.
				Ray r = cam.get_ray(u, v); // Ray from the origin to -z plane. Factor of u in the horizontal, factor of v in the vertical.
				col += color(r, world, 0); // Color returned from the object the ray intersected with.
			}
			col /= float(ns);
			col = Vector3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			file << ir << " " << ig << " " << ib << "\n";
		}
	}

	file.close();
	return 0;
}