#ifndef FRAGMENT_SHADING_HPP
#define FRAGMENT_SHADING_HPP

#include "types.hpp"
#include "rgba.hpp"
#include "vec2i.hpp"

struct FSIn
{
    Vec2i pixel;
    float_psp depth;

    FSIn();
};

struct FSOut
{
    RGBA color;
    float_psp depth;

    FSOut(RGBA color, float_psp depth);
};

typedef FSOut (*FragmentShader)(const FSIn &input);

#endif