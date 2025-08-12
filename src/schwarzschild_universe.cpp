#include "schwarzschild_universe2d.h"

#include "constants.h"

#include <iostream>

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole& blackhole, const std::vector<Ray2D>& rays)
    : _blackhole(blackhole), _rays(rays)
{ }

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole &blackhole)
    : _blackhole(blackhole)
{ }

SchwarzschildUniverse::~SchwarzschildUniverse()
{

}

const BlackHole &SchwarzschildUniverse::get_blackhole() const { return _blackhole; }

const std::vector<Ray2D>& SchwarzschildUniverse::get_rays() const { return _rays; }

void SchwarzschildUniverse::add_ray(Ray2D& ray)
{
	_rays.push_back(ray);
}

// const std::vector<std::tuple<float, Ray2D>>& SchwarzschildUniverse::get_ray_trails() const { return _ray_trails; }

void SchwarzschildUniverse::update(const double dlambda)
{
    for (auto& ray : _rays)
    {
        double r0 = ray.transform.r;
        if (r0 <= _blackhole.r_s)
        {
            if (ray.trail.size())
            {
                ray.trail.erase(ray.trail.begin());
            }
            continue;
        }
        _update_ray_rk4(ray, dlambda);

        ray.x = ray.transform.r * cos(ray.transform.phi);
        ray.y = ray.transform.r * sin(ray.transform.phi);
        if (ray.transform.r > 0)
        {
            ray.trail.push_back({ ray.x, ray.y });
            if (ray.trail.size() > MAX_TRAIL)
            {
                ray.trail.erase(ray.trail.begin());
            }
        }
    }
}

std::optional<PolarTransform2D> SchwarzschildUniverse::null_geodesic(const PolarTransform2D &k, const double E)
{
	double dr   = k.d_r;
	double dphi = k.d_phi;

	double f = 1.0 - _blackhole.r_s / k.r;
    
    if (f <= 0)
    {
        // TODO: Handle r <= r_s.
        // As the ray reaches the event horizon (r = r_s), dt_dλ goes to infinity which, understandably, causes some issues.
        return std::optional<PolarTransform2D>();
    }

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

void SchwarzschildUniverse::_update_ray_rk4(Ray2D &ray, const double dlambda)
{
    const std::optional<PolarTransform2D> k1 = null_geodesic(ray.transform, ray.E);
    if (!k1.has_value())
    {
        ray.transform = PolarTransform2D();
        return;
    }

    const PolarTransform2D y2 = ray.transform + k1.value() * (dlambda * 0.5);
    const std::optional<PolarTransform2D> k2 = null_geodesic(y2, ray.E);
    if (!k2.has_value())
    {
        ray.transform = PolarTransform2D();
        return;
    }

    const PolarTransform2D y3 = ray.transform + k2.value() * (dlambda * 0.5);
    const std::optional<PolarTransform2D> k3 = null_geodesic(y3, ray.E);
    if (!k3.has_value())
    {
        ray.transform = PolarTransform2D();
        return;
    }

    const PolarTransform2D y4 = ray.transform + k3.value() * dlambda;
    const std::optional<PolarTransform2D> k4 = null_geodesic(y4, ray.E);
    if (!k4.has_value())
    {
        ray.transform = PolarTransform2D();
        return;
    }

	ray.transform += (dlambda / 6.0) * (k1.value() + 2 * k2.value() + 2 * k3.value() + k4.value());
}
