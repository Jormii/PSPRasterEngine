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

Vec4f RGBA::AsVec4f() const
{
    return Vec4f{
        static_cast<float_psp>(r),
        static_cast<float_psp>(g),
        static_cast<float_psp>(b),
        static_cast<float_psp>(opacity)};
}

RGBA RGBA::Vec4fAsRGBA(const Vec4f &v)
{
    return RGBA{
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.x))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.y))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.z))),
        static_cast<uint8_psp>(255.0f * std::max(0.0f, std::min(1.0f, v.w)))};
}

RGBA RGBA::Grayscale(float_psp scalar)
{
    float_psp clipped{scalar};
    if (scalar > 1.0f)
    {
        clipped = 1.0f;
    }
    else if (scalar < 0.0f)
    {
        clipped = 0.0f;
    }

    uint8_psp grey{static_cast<uint8_psp>(255.0f * clipped)};
    return RGBA(grey, grey, grey);
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

RGBA RGBA::BarycentricInterpolation(const RGBA &c1, const RGBA &c2, const RGBA &c3, const Vec3f &weights)
{
    Vec4f interpolatedVec4{Vec4f::BarycentricInterpolation(
        c1.AsVec4f(), c2.AsVec4f(), c3.AsVec4f(),
        weights)};

    return RGBA{
        static_cast<uint8_psp>(interpolatedVec4.x),
        static_cast<uint8_psp>(interpolatedVec4.y),
        static_cast<uint8_psp>(interpolatedVec4.z),
        static_cast<uint8_psp>(interpolatedVec4.w)};
}

RGBA operator*(float_psp scalar, const RGBA &color)
{
    Vec4f asVec4{scalar * color.AsVec4f()};
    for (size_t i{0}; i < 4; ++i)
    {
        asVec4(i) = std::max(0.0f, std::min(255.0f, asVec4(i)));
    }

    return RGBA{
        static_cast<uint8_psp>(asVec4.x),
        static_cast<uint8_psp>(asVec4.y),
        static_cast<uint8_psp>(asVec4.z),
        static_cast<uint8_psp>(asVec4.w)};
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