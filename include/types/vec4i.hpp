#ifndef VEC_4I_HPP
#define VEC_4I_HPP

#include <iostream>

#include "types.hpp"

struct Vec4i
{
    int_psp x;
    int_psp y;
    int_psp z;
    int_psp w;

    Vec4i();
    Vec4i(int_psp x, int_psp y, int_psp z, int_psp w);

    friend std::ostream &operator<<(std::ostream &out, const Vec4i &v);
};

#endif
