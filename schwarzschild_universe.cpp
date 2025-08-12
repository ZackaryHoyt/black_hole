#include "schwarzschild_universe.h"

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole& blackhole, std::vector<Ray>* rays)
    : _blackhole(blackhole), _rays(*rays)
{ }

SchwarzschildUniverse::SchwarzschildUniverse(const BlackHole &blackhole)
    : _blackhole(blackhole)
{ }

SchwarzschildUniverse::~SchwarzschildUniverse()
{

}

const BlackHole &SchwarzschildUniverse::get_blackhole() const
{
    return _blackhole;
}

const std::vector<Ray> *SchwarzschildUniverse::get_rays() const
{
    return &_rays;
}

const std::vector<std::tuple<float, Ray>> *SchwarzschildUniverse::get_ray_trails() const
{
    return &_ray_trails;
}

void SchwarzschildUniverse::update(const float dt)
{
    for (auto& ray : _rays)
    {
        _update_ray_rk4(ray, dt);
    }
}

void SchwarzschildUniverse::_update_ray_rk4(Ray &ray, const float dt)
{
	double y0[4] = { ray.r, ray.phi, ray.dr, ray.dphi };
	double k1[4], k2[4], k3[4], k4[4], temp[4];

	geodesic(ray, k1, _blackhole.r_s);
	addState(y0, k1, dt/2.0, temp);
	Ray r2 = ray; r2.r=temp[0]; r2.phi=temp[1]; r2.dr=temp[2]; r2.dphi=temp[3];
	geodesic(r2, k2, _blackhole.r_s);

	addState(y0, k2, dt/2.0, temp);
	Ray r3 = ray; r3.r=temp[0]; r3.phi=temp[1]; r3.dr=temp[2]; r3.dphi=temp[3];
	geodesic(r3, k3, _blackhole.r_s);

	addState(y0, k3, dt, temp);
	Ray r4 = ray; r4.r=temp[0]; r4.phi=temp[1]; r4.dr=temp[2]; r4.dphi=temp[3];
	geodesic(r4, k4, _blackhole.r_s);

	ray.r    += (dt/6.0)*(k1[0] + 2*k2[0] + 2*k3[0] + k4[0]);
	ray.phi  += (dt/6.0)*(k1[1] + 2*k2[1] + 2*k3[1] + k4[1]);
	ray.dr   += (dt/6.0)*(k1[2] + 2*k2[2] + 2*k3[2] + k4[2]);
	ray.dphi += (dt/6.0)*(k1[3] + 2*k2[3] + 2*k3[3] + k4[3]);
}
