#include "rgba.hpp"

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

unsigned int RGBA::AsUint() const
{
    return opacity + (8 << b) + (16 << g) + (24 << r);
}

RGBA RGBA::Vec4fAsRGBA(const Vec4f &v)
{
    return RGBA{
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.x))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.y))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.z))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.w)))};
}

std::ostream &operator<<(std::ostream &out, const RGBA &rgba)
{
    out << "(" << static_cast<int_psp>(rgba.r) << ", " << static_cast<int_psp>(rgba.g) << ", "
        << static_cast<int_psp>(rgba.b) << ", " << static_cast<int_psp>(rgba.opacity) << ")";
    return out;
}