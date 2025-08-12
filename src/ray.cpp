#include "ray.h"
#include "constants.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>


PolarTransform2D geodesic(const PolarTransform2D& k, const double E, const double r_s)
{
	double dr   = k.d_r;
	double dphi = k.d_phi;

	double f = 1.0 - r_s / k.r;

	// dr/dλ = dr
	
	// dφ/dλ = dphi

	// d²r/dλ² from Schwarzschild null geodesic:
	double dt_dλ = E / f;
	double d2r_dlambda2 = 
		- (r_s / (2 * k.r * k.r)) * f * (dt_dλ * dt_dλ)
		+ (r_s / (2 * k.r * k.r * f)) * (dr * dr)
		+ (k.r - r_s) * (dphi * dphi);

	// d²φ/dλ² = -2*(dr * dphi) / r
	double d2phi_dlambda2 = -2.0 * dr * dphi / k.r;

	return PolarTransform2D(k.d_r, k.d_phi, d2r_dlambda2, d2phi_dlambda2);
}

void addState(const double a[4], const double b[4], double factor, double out[4])
{
	for (int i = 0; i < 4; i++)
	{
		out[i] = a[i] + b[i] * factor;
	}
}

Ray::Ray(glm::vec2 pos, glm::vec2 dir, const BlackHole& body)
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

void Ray::draw(const std::vector<Ray>& rays)
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
			continue;

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

void Ray::rk4Step(double dlambda, double r_s)
{
    const PolarTransform2D k1 = geodesic(transform, E, r_s);

    const PolarTransform2D y2 = transform + k1 * (dlambda * 0.5);
    const PolarTransform2D k2 = geodesic(y2, E, r_s);

    const PolarTransform2D y3 = transform + k2 * (dlambda * 0.5);
    const PolarTransform2D k3 = geodesic(y3, E, r_s);

    const PolarTransform2D y4 = transform + k3 * dlambda;
    const PolarTransform2D k4 = geodesic(y4, E, r_s);

	transform += (dlambda / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
}

void Ray::step(double dlambda, double rs)
{
	// 1) integrate (r,φ,dr,dφ)
	if(transform.r <= rs) return; // stop if inside the event horizon
	rk4Step(dlambda, rs);

	// 2) convert back to cartesian x,y
	x = transform.r * cos(transform.phi);
	y = transform.r * sin(transform.phi);

	// 3) record the trail
	trail.push_back({ float(x), float(y) });
}
