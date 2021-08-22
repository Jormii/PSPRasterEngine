#ifndef VEC_2I_HPP
#define VEC_2I_HPP

#include <iostream>

#include "types.hpp"

struct Vec2i
{
    int_psp x;
    int_psp y;

    Vec2i();
    Vec2i(int_psp x, int_psp y);
    Vec2i(const Vec2i &v);

    static int_psp Dot(const Vec2i &u, const Vec2i &v);

    friend Vec2i operator-(const Vec2i &u, const Vec2i &v);

    friend std::ostream &operator<<(std::ostream &out, const Vec2i &v);
};

#endif