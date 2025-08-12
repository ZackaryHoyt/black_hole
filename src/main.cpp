#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "constants.h"
#include "engine.h"
#include "blackhole.h"
#include "ray2d.h"
#include "schwarzschild_universe.h"


int main()
{
	BlackHole sagittarius_a_star(glm::vec3(0.0f, 0.0f, 0.0f), 8.54e36);

	std::vector<Ray2D> rays;

	for (int i = -16; i <= 16; ++i)
	{
		rays.emplace_back(glm::vec2(-1e11, (i / 4.0) * 1e10), glm::vec2(C, 0.0f), sagittarius_a_star);
	}

	SchwarzschildUniverse universe(sagittarius_a_star, &rays);

	Engine engine;
	while (!glfwWindowShouldClose(engine.window))
	{
		engine.run();
		sagittarius_a_star.draw();

		universe.update(1);
		Ray2D::draw(*universe.get_rays());

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	return 0;
}
