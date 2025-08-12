#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "blackhole.h"

struct Ray;
void addState(const double a[4], const double b[4], double factor, double out[4]);
void geodesicRHS(const Ray& ray, double rhs[4], double rs);

struct Ray
{
	// -- cartesian coords -- //
	double x;   double y;
	// -- polar coords -- //
	double r;   double phi;
	double dr;  double dphi;
	std::vector<glm::vec2> trail; // trail of points
	double E, L; // conserved quantities

	Ray(glm::vec2 pos, glm::vec2 dir, const BlackHole& body);

	void draw(const std::vector<Ray>& rays);

	void rk4Step(Ray& ray, double dλ, double rs);

	void step(double dλ, double rs);
};
