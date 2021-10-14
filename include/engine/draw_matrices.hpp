#ifndef DRAW_MATRICES_HPP
#define DRAW_MATRICES_HPP

#include "mat4f.hpp"

struct DrawMatrices
{
    const Mat4f &model;
    const Mat4f &view;
    const Mat4f &projection;
    const Mat4f mv;
    const Mat4f mvp;
    Mat4f normalMat;

    DrawMatrices(const Mat4f &model, const Mat4f &view, const Mat4f &projection);
};

#endif
