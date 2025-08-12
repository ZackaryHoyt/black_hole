#pragma once

#include "ray2d.h"

#include <vector>
#include <tuple>

class SchwarzschildUniverse
{
private:
	const BlackHole _blackhole;
	std::vector<Ray2D> _rays;

	std::vector<std::tuple<float, Ray2D>> _ray_trails;

	float _elapsed_time = 0;

public:
	SchwarzschildUniverse(const BlackHole& blackhole, std::vector<Ray2D>* rays);
	SchwarzschildUniverse(const BlackHole& blackhole);

	~SchwarzschildUniverse();

	const BlackHole& get_blackhole() const;
	
	const std::vector<Ray2D>& get_rays() const;

	void add_rays(Ray2D& ray);

	const std::vector<std::tuple<float, Ray2D>>& get_ray_trails() const;

	void update(const float dlambda);

	PolarTransform2D null_geodesic(const PolarTransform2D& k, const double E);

private:
	void _update_ray_rk4(Ray2D& ray, const float dlambda);
};
