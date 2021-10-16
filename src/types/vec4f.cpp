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

Vec4f Vec4f::Interpolate(const Vec4f &u, const Vec4f &v, float_psp t)
{
    return (1.0 - t) * u + t * v;
}

Vec4f Vec4f::BarycentricInterpolation(const Vec4f &u, const Vec4f &v, const Vec4f &w, const Vec3f &weights)
{
    return weights.x * u + weights.y * v + weights.z * w;
}

float_psp &Vec4f::operator()(size_t i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        exit(-1);
    }
}

float_psp Vec4f::operator()(size_t i) const
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        exit(-1);
    }
}

Vec4f operator+(const Vec4f &u, const Vec4f &v)
{
    return Vec4f{
        u.x + v.x,
        u.y + v.y,
        u.z + v.z,
        u.w + v.w};
}

Vec4f operator-(const Vec4f &u, const Vec4f &v)
{
    return Vec4f{
        u.x - v.x,
        u.y - v.y,
        u.z - v.z,
        u.w - v.w};
}

Vec4f operator*(float_psp scalar, const Vec4f &v)
{
    return Vec4f{
        scalar * v.x,
        scalar * v.y,
        scalar * v.z,
        scalar * v.w};
}

Vec4f operator*(const Vec4f &u, const Vec4f &v)
{
    return Vec4f{
        u.x * v.x,
        u.y * v.y,
        u.z * v.z,
        u.w * v.w};
}

std::ostream &operator<<(std::ostream &out, const Vec4f &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return out;
}