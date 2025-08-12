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

	for (int i = -16; i <= 16; ++i)
	{
		rays.emplace_back(glm::vec2(-1e11, (i / 4.0) * 1e10), glm::vec2(C, 0.0f), sagittarius_a_star);
	}
	std::cout << rays.size() << std::endl;

	Engine engine;
	while (!glfwWindowShouldClose(engine.window))
	{
		engine.run();
		sagittarius_a_star.draw();

		for (auto& ray : rays)
		{
			ray.step(1, sagittarius_a_star.r_s);
		}
		Ray::draw(rays);

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}

	return 0;
}
