#include "schwarzschild_universe2d.h"

#include <iostream>

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole& blackhole, std::vector<Ray2D>* rays)
    : _blackhole(blackhole), _rays(*rays)
{ }

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole &blackhole)
    : _blackhole(blackhole)
{ }

SchwarzschildUniverse::~SchwarzschildUniverse()
{

}

const BlackHole &SchwarzschildUniverse::get_blackhole() const { return _blackhole; }

const std::vector<Ray2D>& SchwarzschildUniverse::get_rays() const { return _rays; }

void SchwarzschildUniverse::add_rays(Ray2D& ray)
{
	_rays.push_back(ray);
}

const std::vector<std::tuple<float, Ray2D>>& SchwarzschildUniverse::get_ray_trails() const { return _ray_trails; }

void SchwarzschildUniverse::update(const float dlambda)
{
    const size_t MAX_TRAIL = 4000;
    for (auto& ray : _rays)
    {
        if (ray.transform.r <= _blackhole.r_s)
        {
            ray.trail.push_back({ ray.x, ray.y });
            ray.trail.erase(ray.trail.begin());
            continue;
        }
        _update_ray_rk4(ray, dlambda);
        ray.x = ray.transform.r * cos(ray.transform.phi);
        ray.y = ray.transform.r * sin(ray.transform.phi);
        ray.trail.push_back({ ray.x, ray.y });
        if (ray.trail.size() > MAX_TRAIL)
        {
            ray.trail.erase(ray.trail.begin());
        }
    }
}

PolarTransform2D SchwarzschildUniverse::null_geodesic(const PolarTransform2D &k, const double E)
{
	double dr   = k.d_r;
	double dphi = k.d_phi;

	double f = 1.0 - _blackhole.r_s / k.r;

	// dr/dλ = dr
	
	// dφ/dλ = dphi

	// d²r/dλ² from Schwarzschild null geodesic:
	double dt_dλ = E / f;
	double d2r_dlambda2 = 
		- (_blackhole.r_s / (2 * k.r * k.r)) * f * (dt_dλ * dt_dλ)
		+ (_blackhole.r_s / (2 * k.r * k.r * f)) * (dr * dr)
		+ (k.r - _blackhole.r_s) * (dphi * dphi);

	// d²φ/dλ² = -2*(dr * dphi) / r
	double d2phi_dlambda2 = -2.0 * dr * dphi / k.r;

	return PolarTransform2D(k.d_r, k.d_phi, d2r_dlambda2, d2phi_dlambda2);
}

void SchwarzschildUniverse::_update_ray_rk4(Ray2D &ray, const float dlambda)
{
    const PolarTransform2D k1 = null_geodesic(ray.transform, ray.E);

    const PolarTransform2D y2 = ray.transform + k1 * (dlambda * 0.5);
    const PolarTransform2D k2 = null_geodesic(y2, ray.E);

    const PolarTransform2D y3 = ray.transform + k2 * (dlambda * 0.5);
    const PolarTransform2D k3 = null_geodesic(y3, ray.E);

    const PolarTransform2D y4 = ray.transform + k3 * dlambda;
    const PolarTransform2D k4 = null_geodesic(y4, ray.E);

	ray.transform += (dlambda / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
}
