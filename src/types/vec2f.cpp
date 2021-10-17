#include "vec2f.hpp"

Vec2f::Vec2f() : x{0.0f}, y{0.0f}
{
}

Vec2f::Vec2f(float_psp x, float_psp y) : x{x}, y{y}
{
}

Vec2f::Vec2f(const Vec2f &v) : x{v.x}, y{v.y}
{
}

float_psp Vec2f::Dot(const Vec2f &u, const Vec2f &v)
{
    return (u.x * v.x) + (u.y * v.y);
}

Vec2f Vec2f::BarycentricInterpolation(const Vec2f &u, const Vec2f &v, const Vec2f &w, const Vec3f &weights)
{
    return weights.x * u + weights.y * v + weights.z * w;
}

Vec2f operator+(const Vec2f &u, const Vec2f &v)
{
    return Vec2f{u.x + v.x, u.y + v.y};
}

Vec2f operator-(const Vec2f &u, const Vec2f &v)
{
    return Vec2f{u.x - v.x, u.y - v.y};
}

Vec2f operator*(float_psp scalar, const Vec2f &v)
{
    return Vec2f{scalar * v.x, scalar * v.y};
}

std::ostream &operator<<(std::ostream &out, const Vec2f &v)
{
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}