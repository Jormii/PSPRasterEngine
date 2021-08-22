#include "vec2i.hpp"

Vec2i::Vec2i() : x{0}, y{0}
{
}

Vec2i::Vec2i(int_psp x, int_psp y) : x{x}, y{y}
{
}

Vec2i::Vec2i(const Vec2i &v) : x{v.x}, y{v.y}
{
}

int_psp Vec2i::Dot(const Vec2i &u, const Vec2i &v)
{
    return (u.x * v.x) + (u.y * v.y);
}

Vec2i operator-(const Vec2i &u, const Vec2i &v)
{
    return Vec2i{
        u.x - v.x,
        u.y - v.y};
}

std::ostream &operator<<(std::ostream &out, const Vec2i &v)
{
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}