#include "ray2d.h"

#include <algorithm>
#include <cmath>


Ray2D::Ray2D(glm::vec2 pos, glm::vec2 dir, const BlackHole& body)
	: x(pos.x), y(pos.y)
{
	// Define initial ray kinematics.
	double r = sqrt(x * x + y * y);
	double phi = atan2(y, x);
	double d_r = dir.x * cos(phi) + dir.y * sin(phi);
	double d_phi = (-dir.x * sin(phi) + dir.y * cos(phi) ) / r;
	transform = PolarTransform2D(r, phi, d_r, d_phi);
	
	// step 3) store conserved quantities
	L = r * r * transform.d_phi;
	double f = 1.0 - body.r_s / r;
	double dt_dlambda = sqrt((d_r * d_r) / (f * f) + (r * r * d_phi * d_phi) / f);
	E = f * dt_dlambda;

	// step 4) start trail :
	trail.push_back({x, y});
}
