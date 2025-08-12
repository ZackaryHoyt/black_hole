#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "constants.h"
#include "engine.h"
#include "blackhole.h"
#include "ray.h"


int main()
{
	BlackHole sagittarius_a_star(glm::vec3(0.0f, 0.0f, 0.0f), 8.54e36);

	std::vector<Ray> rays;
	rays.emplace_back(glm::vec2(-1e11, 3.27606302719999999e10), glm::vec2(C, 0.0f), sagittarius_a_star);

	Engine engine;
	while (!glfwWindowShouldClose(engine.window))
	{
		engine.run();
		sagittarius_a_star.draw();

		for (auto& ray : rays)
		{
			ray.step(0.1, sagittarius_a_star.r_s);
			ray.draw(rays);
		}

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	return 0;
}
