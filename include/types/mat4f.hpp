#ifndef MAT_4F_HPP
#define MAT_4F_HPP

#include <iostream>

#include "vec4f.hpp"

struct Mat4f
{
    Vec4f r1;
    Vec4f r2;
    Vec4f r3;
    Vec4f r4;

    Mat4f();
    Mat4f(const Vec4f &r1, const Vec4f &r2, const Vec4f &r3, const Vec4f &r4);
    Mat4f(const Mat4f &m);

    Mat4f Transpose() const;

    friend Vec4f operator*(const Mat4f &m, const Vec4f &v);
    friend Mat4f operator*(const Mat4f &a, const Mat4f &b);

    friend std::ostream &operator<<(std::ostream &out, const Mat4f &m);

    // Static variables
    static const Mat4f IDENTITY;
};

#endif