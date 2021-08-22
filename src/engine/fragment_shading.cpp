#include "fragment_shading.hpp"

FSIn::FSIn() : pixel{}, depth{}
{
}

FSOut::FSOut(RGBA color, float_psp depth) : color{color}, depth{depth}
{
}