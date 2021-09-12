#ifndef VEC_3F_HPP
#define VEC_3F_HPP

#include <iostream>

#include "types.hpp"

struct Vec3f
{
    float_psp x;
    float_psp y;
    float_psp z;

    Vec3f();
    Vec3f(float_psp x, float_psp y, float_psp z);
    Vec3f(const Vec3f &v);

    float_psp Magnitude() const;
    Vec3f &Normalize();
    Vec3f Normalized() const;

    static float_psp Dot(const Vec3f &u, const Vec3f &v);
    static Vec3f Cross(const Vec3f &u, const Vec3f &v);
    static Vec3f BarycentricInterpolation(const Vec3f &u, const Vec3f &v, const Vec3f &w, const Vec3f &weights);

    Vec3f operator-() const;
    float_psp &operator()(size_t i);
    float_psp operator()(size_t i) const;
    friend Vec3f operator+(const Vec3f &u, const Vec3f &v);
    friend Vec3f operator-(const Vec3f &u, const Vec3f &v);
    friend Vec3f operator*(float_psp scalar, const Vec3f &v);
    friend Vec3f operator/(const Vec3f &v, float_psp scalar);

    friend std::ostream &operator<<(std::ostream &out, const Vec3f &v);
};

#endif
