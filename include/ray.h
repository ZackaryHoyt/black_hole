#pragma once
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "blackhole.h"
#include "polar_transform2d.h"

struct Ray;
PolarTransform2D geodesic(const PolarTransform2D& k, const double E, const double r_s);

struct Ray
{
	// -- cartesian coords -- //
	double x;   double y;
	PolarTransform2D transform;
	std::vector<glm::vec2> trail; // trail of points
	double E, L; // conserved quantities

	Ray(glm::vec2 pos, glm::vec2 dir, const BlackHole& body);

	static void draw(const std::vector<Ray>& rays);

	void rk4Step(double dlambda, double rs);

	void step(double dlambda, double rs);
};
