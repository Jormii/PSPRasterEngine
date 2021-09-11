#ifndef MAT_3F_HPP
#define MAT_3F_HPP

#include <iostream>

#include "vec3f.hpp"

struct Mat3f
{
    Vec3f r1;
    Vec3f r2;
    Vec3f r3;

    Mat3f();
    Mat3f(const Vec3f &r1, const Vec3f &r2, const Vec3f &r3);
    Mat3f(const Mat3f &m);

    float_psp Determinant() const;

    Vec3f &operator()(size_t i);
    Vec3f operator()(size_t i) const;
    float_psp &operator()(size_t i, size_t j);
    float_psp operator()(size_t i, size_t j) const;

    friend std::ostream &operator<<(std::ostream &out, const Mat3f &m);
};

#endif