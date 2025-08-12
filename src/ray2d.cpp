#include "ray2d.h"
#include "constants.h"

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

void Ray2D::draw(const std::vector<Ray2D>& rays)
{
	// draw current ray positions as points
	glPointSize(2.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (const auto& ray : rays)
	{
		glVertex2f(ray.x, ray.y);
	}
	glEnd();

	// turn on blending for the trails
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);

	// draw each trail with fading alpha
	for (const auto& ray : rays)
	{
		size_t N = ray.trail.size();

		if (N < 2)
		{
			continue;
		}

		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < N; ++i)
		{
			// older points (i=0) get alpha≈0, newer get alpha≈1
			float alpha = float(i) / float(N - 1);
			glColor4f(1.0f, 1.0f, 1.0f, std::max(alpha, 0.05f));
			glVertex2f(ray.trail[i].x, ray.trail[i].y);
		}
		glEnd();
	}

	glDisable(GL_BLEND);
}

