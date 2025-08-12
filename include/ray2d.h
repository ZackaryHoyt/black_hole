#pragma once
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "blackhole.h"
#include "polar_transform2d.h"


struct Ray2D
{
	double x, y; // cartesian coordinates
	PolarTransform2D transform;
	std::vector<glm::vec2> trail; // trail of points
	double E, L; // conserved quantities

	Ray2D(glm::vec2 pos, glm::vec2 dir, const BlackHole& body);

	static void draw(const std::vector<Ray2D>& rays);
};
