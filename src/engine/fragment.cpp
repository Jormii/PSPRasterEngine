#include "fragment.hpp"

Fragment::Fragment(int_psp xScreenCoord, int_psp yScreenCoord, float_psp depth, const RGBA &color)
    : xScreenCoord{xScreenCoord}, yScreenCoord{yScreenCoord}, depth{depth}, color{color}
{
}
