#ifndef FRAGMENT
#define FRAGMENT

#include "types.hpp"
#include "vec3f.hpp"
#include "rgba.hpp"

/// Struct that stores a fragment's data
struct Fragment
{
    int_psp xScreenCoord; /* X viewport coordinate */
    int_psp yScreenCoord; /* Y viewport coordinate */
    float_psp depth;      /* Fragment's depth. Value in range [-1, 1] */
    RGBA color;

    Fragment(int_psp xScreenCoord, int_psp yScreenCoord, float_psp depth, const RGBA &color);
};

#endif