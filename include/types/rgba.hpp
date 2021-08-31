#ifndef RGBA_HPP
#define RGBA_HPP

#include <iostream>

#include "types.hpp"

struct RGBA
{
    uint8_psp r;
    uint8_psp g;
    uint8_psp b;
    uint8_psp opacity;

    RGBA();
    RGBA(uint8_psp r, uint8_psp g, uint8_psp b, uint8_psp opacity);
    RGBA(const RGBA &color);

    static RGBA Interpolate(const RGBA &c1, const RGBA &c2, float_psp t);

    friend std::ostream &operator<<(std::ostream &out, const RGBA &rgba);

#ifndef PSP
    operator bool() const;
#endif
};

#endif
