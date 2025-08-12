#pragma once

#include "ray.h"

#include <vector>
#include <tuple>

class SchwarzschildUniverse
{
private:
	const BlackHole _blackhole;
	std::vector<Ray> _rays;

	std::vector<std::tuple<float, Ray>> _ray_trails;

	float _elapsed_time = 0;

public:
	SchwarzschildUniverse(const BlackHole& blackhole, std::vector<Ray>* rays);
	SchwarzschildUniverse(const BlackHole& blackhole);

	~SchwarzschildUniverse();

	const BlackHole& get_blackhole() const;
	
	const std::vector<Ray>* get_rays() const;

	const std::vector<std::tuple<float, Ray>>* get_ray_trails() const;

	void update(const float dt);

private:
	void _update_ray_rk4(Ray& ray, const float dt);
};
