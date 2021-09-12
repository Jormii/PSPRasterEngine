#include "fragment.hpp"

Fragment::Fragment(int_psp xScreenCoord, int_psp yScreenCoord, float_psp depth, const Vec3f &normal, const RGBA &color)
    : xScreenCoord{xScreenCoord}, yScreenCoord{yScreenCoord}, depth{depth}, normal{normal}, color{color}
{
}
