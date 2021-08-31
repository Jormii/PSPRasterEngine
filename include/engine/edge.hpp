#ifndef EDGE_HPP
#define EDGE_HPP

#include "types.hpp"
#include "vec2f.hpp"

struct EdgeFunction
{
    // Edge function: e(x, y): ax + by + c = 0
    float_psp a;
    float_psp b;
    float_psp c;

    EdgeFunction(float_psp a, float_psp b, float_psp c);

    float_psp Evaluate(const Vec2f &p) const;

    static EdgeFunction FromPoints(const Vec2f &p, const Vec2f &q);
};

#endif