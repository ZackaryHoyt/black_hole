#include <vector>
#include <iostream>

#include "constants.h"
#include "blackhole.h"
#include "engine2d.h"
#include "ray2d.h"
#include "schwarzschild_universe2d.h"


int main()
{
	BlackHole sagittarius_a_star(glm::vec3(0.0f, 0.0f, 0.0f), 8.54e36);

	std::vector<Ray2D> rays;

	const int n_rays = 1 << 11;

	const double ray_interval = 8.75e9 / (n_rays >> 3);

	for (int i = -n_rays >> 1; i <= n_rays >> 1; ++i)
	{
		rays.emplace_back(glm::vec2(-1e11, i * ray_interval), glm::vec2(C, 0.0f), sagittarius_a_star);
	}

	SchwarzschildUniverse universe(sagittarius_a_star, rays);

	Engine engine;
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
