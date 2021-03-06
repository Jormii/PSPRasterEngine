#include "vec4i.hpp"

Vec4i::Vec4i() : x(0), y{0}, z{0}, w{0}
{
}

Vec4i::Vec4i(int_psp x, int_psp y, int_psp z, int_psp w) : x{x}, y{y}, z{z}, w{w}
{
}

std::ostream &operator<<(std::ostream &out, const Vec4i &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return out;
}