#ifndef DRAW_MATRICES_HPP
#define DRAW_MATRICES_HPP

#include "mat4f.hpp"

struct DrawMatrices
{
    Mat4f v;
    Mat4f mv;
    Mat4f mvp;
    Mat4f n;

    void Init(const Mat4f &m, const Mat4f &v, const Mat4f &p);
};

#endif
