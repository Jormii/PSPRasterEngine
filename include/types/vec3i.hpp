#ifndef VEC_3I_HPP
#define VEC_3I_HPP

#include <iostream>

#include "types.hpp"

struct Vec3i
{
    int_psp x;
    int_psp y;
    int_psp z;

    Vec3i();
    Vec3i(int_psp x, int_psp y, int_psp z);
    Vec3i(const Vec3i &v);

    friend std::ostream &operator<<(std::ostream &out, const Vec3i &v);
};

#endif
