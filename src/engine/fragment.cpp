#include "fragment.hpp"

Fragment::Fragment(int_psp xScreenCoord, int_psp yScreenCoord, float_psp depth, const Vec3f &viewPos, const Vec3f &normal, const Vec4f &color, const Vec2f &uv)
    : xScreenCoord{xScreenCoord}, yScreenCoord{yScreenCoord}, depth{depth}, viewPos{viewPos}, normal{normal}, color{color}, uv{uv}
{
}
