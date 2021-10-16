#ifndef RGBA_HPP
#define RGBA_HPP

#include <iostream>

#include "types.hpp"
#include "vec3f.hpp"
#include "vec4f.hpp"

struct RGBA
{
    uint8_psp r;
    uint8_psp g;
    uint8_psp b;
    uint8_psp opacity;

    RGBA();
    RGBA(uint8_psp r, uint8_psp g, uint8_psp b);
    RGBA(uint8_psp r, uint8_psp g, uint8_psp b, uint8_psp opacity);
    RGBA(const RGBA &color);

    Vec4f AsVec4f() const;

    static RGBA Vec4fAsRGBA(const Vec4f &v);
    static RGBA Grayscale(float_psp scalar);
    static RGBA Interpolate(const RGBA &c1, const RGBA &c2, float_psp t);
    static RGBA BarycentricInterpolation(const RGBA &c1, const RGBA &c2, const RGBA &c3, const Vec3f &weights);

    friend RGBA operator*(float_psp scalar, const RGBA &color);

    friend std::ostream &operator<<(std::ostream &out, const RGBA &rgba);

#ifndef PSP
    operator bool() const;
#endif
};

#endif
