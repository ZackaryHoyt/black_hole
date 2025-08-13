#pragma once

#include <glm/glm.hpp>

struct BlackHole
{
	glm::vec3 p;
	double m;
	double r_s;

	BlackHole(glm::vec3 p, double m);
};
