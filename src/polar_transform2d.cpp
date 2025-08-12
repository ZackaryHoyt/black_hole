#include "polar_transform2d.h"

PolarTransform2D::PolarTransform2D(double r, double phi, double d_r, double d_phi)
    : r(r), phi(phi), d_r(d_r), d_phi(d_phi)
{ }

PolarTransform2D::PolarTransform2D() : PolarTransform2D(0,0,0,0) { }

PolarTransform2D PolarTransform2D::operator+(const PolarTransform2D &other) const
{
    return PolarTransform2D(r + other.r, phi + other.phi, d_r + other.d_r, d_phi + other.d_phi);
}

PolarTransform2D& PolarTransform2D::operator+=(const PolarTransform2D& other)
{
    r += other.r;
    phi += other.phi;
    d_r += other.d_r;
    d_phi += other.d_phi;
    return *this;
}
