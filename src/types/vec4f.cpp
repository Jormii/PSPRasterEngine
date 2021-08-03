#include "vec4f.hpp"

Vec4f::Vec4f() : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f}
{
}

Vec4f::Vec4f(float_psp x, float_psp y, float_psp z, float_psp w) : x{x}, y{y}, z{z}, w{w}
{
}

Vec4f::Vec4f(const Vec3f &v, float_psp w) : x{v.x}, y{v.y}, z{v.z}, w{w}
{
}

Vec4f::Vec4f(const Vec4f &v) : x{v.x}, y{v.y}, z{v.z}, w{v.w}
{
}

Vec3f Vec4f::DivideByW() const
{
    return Vec3f{
        x / w,
        y / w,
        z / w};
}

float_psp Vec4f::Dot(const Vec4f &u, const Vec4f &v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z) + (u.w * v.w);
}

std::ostream &operator<<(std::ostream &out, const Vec4f &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return out;
}