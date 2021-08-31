#ifndef FRAGMENT_SHADING_HPP
#define FRAGMENT_SHADING_HPP

#include "fragment.hpp"

#include "types.hpp"
#include "rgba.hpp"

struct FSOut
{
    RGBA color;
    float_psp depth;

    FSOut();
};

typedef void (*FragmentShader)(const Fragment &fragment, FSOut &out);

#endif