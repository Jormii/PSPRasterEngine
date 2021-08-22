#include "vec3i.hpp"

Vec3i::Vec3i() : x{0}, y{0}, z{0}
{
}

Vec3i::Vec3i(int_psp x, int_psp y, int_psp z) : x{x}, y{y}, z{z}
{
}

Vec3i::Vec3i(const Vec3i &v) : x{v.x}, y{v.y}, z{v.z}
{
}

std::ostream &operator<<(std::ostream &out, const Vec3i &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}