#ifndef VEC_4F_HPP
#define VEC_4F_HPP

#include <iostream>

#include "types.hpp"
#include "vec3f.hpp"

struct Vec4f
{
    float_psp x;
    float_psp y;
    float_psp z;
    float_psp w;

    Vec4f();
    Vec4f(float_psp x, float_psp y, float_psp z, float_psp w);
    Vec4f(const Vec3f &v, float_psp w);
    Vec4f(const Vec4f &v);

    Vec3f DivideByW() const;

    static float_psp Dot(const Vec4f &u, const Vec4f &v);
    static Vec4f Interpolate(const Vec4f &u, const Vec4f &v, float_psp t);
    static Vec4f BarycentricInterpolation(const Vec4f &u, const Vec4f &v, const Vec4f &w, const Vec3f &weights);

    float_psp &operator()(size_t i);
    float_psp operator()(size_t i) const;
    friend Vec4f operator+(const Vec4f &u, const Vec4f &v);
    friend Vec4f operator-(const Vec4f &u, const Vec4f &v);
    friend Vec4f operator*(float_psp scalar, const Vec4f &v);

    friend std::ostream &operator<<(std::ostream &out, const Vec4f &v);
};

#endif
