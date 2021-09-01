#include "rgba.hpp"

#include "vec4f.hpp"

RGBA::RGBA() : r{0}, g{0}, b{0}, opacity{255}
{
}

RGBA::RGBA(uint8_psp r, uint8_psp g, uint8_psp b)
    : r{r}, g{g}, b{b}, opacity{255}
{
}

RGBA::RGBA(uint8_psp r, uint8_psp g, uint8_psp b, uint8_psp opacity)
    : r{r}, g{g}, b{b}, opacity{opacity}
{
}

RGBA::RGBA(const RGBA &color) : r{color.r}, g{color.g}, b{color.b}, opacity{color.opacity}
{
}

RGBA RGBA::Interpolate(const RGBA &c1, const RGBA &c2, float_psp t)
{
    Vec4f c1F{static_cast<float_psp>(c1.r), static_cast<float_psp>(c1.g), static_cast<float_psp>(c1.b), static_cast<float_psp>(c1.opacity)};
    Vec4f c2F{static_cast<float_psp>(c2.r), static_cast<float_psp>(c2.g), static_cast<float_psp>(c2.b), static_cast<float_psp>(c2.opacity)};
    Vec4f interpolated{Vec4f::Interpolate(c1F, c2F, t)};
    return RGBA{
        static_cast<uint8_psp>(interpolated.x),
        static_cast<uint8_psp>(interpolated.y),
        static_cast<uint8_psp>(interpolated.z),
        static_cast<uint8_psp>(interpolated.w)};
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