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

	for (int i = -512; i <= 512; ++i)
	{
		rays.emplace_back(glm::vec2(-1e11, (i / 128.0) * 1e10), glm::vec2(C, 0.0f), sagittarius_a_star);
	}

	SchwarzschildUniverse universe(sagittarius_a_star, &rays);

	Engine engine;
	while (!glfwWindowShouldClose(engine.window))
	{
		engine.run();
		sagittarius_a_star.draw();

		universe.update(.5);
		Ray2D::draw(universe.get_rays());

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	return 0;
}
