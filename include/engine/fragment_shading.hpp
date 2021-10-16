#ifndef FRAGMENT_SHADING_HPP
#define FRAGMENT_SHADING_HPP

#include "fragment.hpp"
#include "draw_matrices.hpp"

#include "types.hpp"

struct FSOut
{
    Vec4f color;
    float_psp depth;
};

typedef void (*FragmentShader)(const DrawMatrices &matrices, const Fragment &fragment, FSOut &out, const bool *activeLights, const PointLight *lights);

#endif