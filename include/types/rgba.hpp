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

    unsigned int AsUint() const;

    static RGBA Vec4fAsRGBA(const Vec4f &v);

    friend std::ostream &operator<<(std::ostream &out, const RGBA &rgba);
};

#endif
