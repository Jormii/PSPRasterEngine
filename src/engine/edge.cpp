#include "edge.hpp"

EdgeFunction::EdgeFunction(float_psp a, float_psp b, float_psp c) : a{a}, b{b}, c{c}
{
}

float_psp EdgeFunction::Evaluate(const Vec2f &p) const
{
    return a * p.x + b * p.y + c;
}

EdgeFunction EdgeFunction::FromPoints(const Vec2f &p, const Vec2f &q)
{
    Vec2f v{q - p};
    return EdgeFunction{
        -v.y,
        v.x,
        v.y * p.x - v.x * p.y};
}