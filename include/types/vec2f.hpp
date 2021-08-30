#ifndef VEC_2F_HPP
#define VEC_2F_HPP

#include <iostream>

#include "types.hpp"

struct Vec2f
{
    float_psp x;
    float_psp y;

    Vec2f();
    Vec2f(float_psp x, float_psp y);
    Vec2f(const Vec2f &v);

    static float_psp Dot(const Vec2f &u, const Vec2f &v);

    friend Vec2f operator-(const Vec2f &u, const Vec2f &v);

    friend std::ostream &operator<<(std::ostream &out, const Vec2f &v);
};

#endif