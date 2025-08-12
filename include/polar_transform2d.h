#pragma once

#include <type_traits>

struct PolarTransform2D
{
    double r, phi;
    double d_r, d_phi;

    PolarTransform2D();
    PolarTransform2D(double r, double phi, double d_r=0, double d_phi=0);

    PolarTransform2D operator+(const PolarTransform2D& other) const;
    PolarTransform2D& operator+=(const PolarTransform2D& other);

    template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    [[nodiscard]] inline PolarTransform2D operator*(T s) const noexcept
    {
        return PolarTransform2D(r * s, phi * s, d_r * s, d_phi * s);
    }

    template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    [[nodiscard]] inline PolarTransform2D operator/(T s) const
    {
        return PolarTransform2D(r / s, phi / s, d_r / s, d_phi / s);
    }
};

template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
[[nodiscard]] inline PolarTransform2D operator*(T s, const PolarTransform2D& o) noexcept { return o * s; }
