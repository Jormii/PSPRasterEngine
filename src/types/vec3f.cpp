#include "vec3f.hpp"

#include <cmath>

Vec3f::Vec3f() : x{0.0f}, y{0.0f}, z{0.0f}
{
}

Vec3f::Vec3f(float_psp x, float_psp y, float_psp z) : x{x}, y{y}, z{z}
{
}

Vec3f::Vec3f(const Vec3f &v) : x{v.x}, y{v.y}, z{v.z}
{
}

float_psp Vec3f::Magnitude() const
{
    return sqrtf(
        (x * x) + (y * y) + (z * z));
}

Vec3f &Vec3f::Normalize()
{
    float_psp magnitude{Magnitude()};
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;

    return *this;
}

Vec3f Vec3f::Normalized() const
{
    Vec3f copy{*this};
    copy.Normalize();

    return copy;
}

float_psp Vec3f::Dot(const Vec3f &u, const Vec3f &v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

Vec3f Vec3f::Cross(const Vec3f &u, const Vec3f &v)
{
    return Vec3f{
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x};
}

Vec3f Vec3f::operator-() const
{
    Vec3f copy{*this};
    copy.x = -copy.x;
    copy.y = -copy.y;
    copy.z = -copy.z;

    return copy;
}

Vec3f operator+(const Vec3f &u, const Vec3f &v)
{
    return Vec3f{
        u.x + v.x,
        u.y + v.y,
        u.z + v.z};
}

Vec3f operator-(const Vec3f &u, const Vec3f &v)
{
    return Vec3f{
        u.x - v.x,
        u.y - v.y,
        u.z - v.z};
}

Vec3f operator*(float_psp scalar, const Vec3f &v)
{
    return Vec3f{
        scalar * v.x,
        scalar * v.y,
        scalar * v.z};
}

Vec3f operator/(const Vec3f &v, float_psp scalar)
{
    return Vec3f{
        v.x / scalar,
        v.y / scalar,
        v.z / scalar};
}

std::ostream &operator<<(std::ostream &out, const Vec3f &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}