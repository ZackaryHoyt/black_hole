#include <vector>
#include <iostream>
#include <random>
#include <iomanip>

#include "constants.h"
#include "blackhole.h"
#include "engine2d.h"
#include "ray2d.h"
#include "schwarzschild_universe2d.h"

void add_ray_stochastic(const double p, SchwarzschildUniverse& universe)
{
	const BlackHole& blackhole = universe.get_blackhole();
	const double SPAWN_RADIUS = 5.0 * blackhole.r_s;

	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_real_distribution<double> angle360(0.0, 2.0 * PI);
	static std::uniform_real_distribution<double> u01(0.2, 0.8);
	static std::uniform_real_distribution<double> ray_spawn(0, 1);

	
	if (p >= ray_spawn(rng))
	{
		// pick a point on the spawn circle
		const double theta = angle360(rng); // [0, 2π)
		const glm::vec2 pos(blackhole.p.x + SPAWN_RADIUS * std::cos(theta), blackhole.p.y + SPAWN_RADIUS * std::sin(theta));

		// inward direction angle (toward BH) + random offset in ±90°
		const double inward = theta + PI; // points from spawn point to BH center
		const double delta  = (u01(rng) - 0.5) * PI; // [-π/2, +π/2]
		const double dir_ang = inward + delta;

		// ray direction (cartesian); scale by c for your constructor
		const glm::vec2 dir(C * std::cos(dir_ang), C * std::sin(dir_ang));
		Ray2D ray(pos, dir, blackhole);
		universe.add_ray(ray);
	}
}

void add_ray_grid(const int n_rays, const double grid_size, SchwarzschildUniverse& universe)
{
	const BlackHole& blackhole = universe.get_blackhole();
	const double ray_offset = 4 * blackhole.r_s;
	const double ray_interval = grid_size / (n_rays >> 5);

	for (int i = -n_rays >> 3; i <= n_rays >> 3; ++i)
	{
		universe.add_ray(Ray2D(glm::vec2(-ray_offset, i * ray_interval), glm::vec2(C, 0), blackhole));
	}

	for (int i = -n_rays >> 3; i <= n_rays >> 3; ++i)
	{
		universe.add_ray(Ray2D(glm::vec2(i * ray_interval, -ray_offset), glm::vec2(0, C), blackhole));
	}

	for (int i = -n_rays >> 3; i <= n_rays >> 3; ++i)
	{
		universe.add_ray(Ray2D(glm::vec2(ray_offset, i * ray_interval), glm::vec2(-C, 0), blackhole));
	}

	for (int i = -n_rays >> 3; i <= n_rays >> 3; ++i)
	{
		universe.add_ray(Ray2D(glm::vec2(i * ray_interval, ray_offset), glm::vec2(0, -C), blackhole));
	}
}

void add_ray_orbital(SchwarzschildUniverse& universe)
{
	const BlackHole& blackhole = universe.get_blackhole();
	const double offset = 19025822720; // As close as I can get it before floating point precision inevitably eats it.
	const double interval = 0.5;
	const double n_rays = 1;
	for (int i = 0; i < n_rays; ++i)
	{
		universe.add_ray(Ray2D(glm::vec2(0, offset + i * interval), glm::vec2(C, 0), blackhole));
	}
}

int main()
{
	BlackHole sagittarius_a_star(glm::vec3(0.0f, 0.0f, 0.0f), 8.54e36);
	SchwarzschildUniverse universe(sagittarius_a_star);

	// add_ray_grid(1 << 10, 5e10, universe);
	add_ray_orbital(universe);

	Engine engine(600,600);
	while (!glfwWindowShouldClose(engine.window))
	{
		engine.run();

		universe.update(1);

		engine.draw(universe);

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	return 0;
}
