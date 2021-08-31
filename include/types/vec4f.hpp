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

    friend Vec4f operator+(const Vec4f &u, const Vec4f &v);
    friend Vec4f operator-(const Vec4f &u, const Vec4f &v);
    friend Vec4f operator*(float_psp scalar, const Vec4f &v);

    friend std::ostream &operator<<(std::ostream &out, const Vec4f &v);
};

#endif
