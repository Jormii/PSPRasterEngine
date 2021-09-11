#ifndef MAT_4F_HPP
#define MAT_4F_HPP

#include <iostream>

#include "vec4f.hpp"
#include "mat3f.hpp"

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
    Mat4f Cofactor() const;
    float_psp Determinant() const;
    Mat3f Minor(size_t i, size_t j) const;
    bool Inverse(Mat4f &out) const;

    Vec4f &operator()(size_t i);
    Vec4f operator()(size_t i) const;
    float_psp &operator()(size_t i, size_t j);
    float_psp operator()(size_t i, size_t j) const;
    friend Mat4f operator*(float_psp scalar, const Mat4f &m);
    friend Vec4f operator*(const Mat4f &m, const Vec4f &v);
    friend Mat4f operator*(const Mat4f &a, const Mat4f &b);

    friend std::ostream &operator<<(std::ostream &out, const Mat4f &m);

    // Static variables
    static const Mat4f IDENTITY;
};

#endif