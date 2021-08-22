#include "rgba.hpp"

RGBA::RGBA() : r{0}, g{0}, b{0}, opacity{255}
{
}

RGBA::RGBA(uint8_psp r, uint8_psp g, uint8_psp b, uint8_psp opacity)
    : r{r}, g{g}, b{b}, opacity{opacity}
{
}

RGBA::RGBA(const RGBA &color) : r{color.r}, g{color.g}, b{color.b}, opacity{color.opacity}
{
}

std::ostream &operator<<(std::ostream &out, const RGBA &rgba)
{
    out << "(" << static_cast<int_psp>(rgba.r) << ", " << static_cast<int_psp>(rgba.g) << ", "
        << static_cast<int_psp>(rgba.b) << ", " << static_cast<int_psp>(rgba.opacity) << ")";
    return out;
}

#ifndef PSP
RGBA::operator bool() const
{
    int_psp rgb_sum = r + g + b;
    return rgb_sum != 0;
}
#endif