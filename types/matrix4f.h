#ifndef MATRIX_4F_H
#define MATRIX_4F_H

#include "./types.h"
#include "./vector4f.h"

typedef struct Matrix4f_t
{
    Vector4f r1;
    Vector4f r2;
    Vector4f r3;
    Vector4f r4;
} Matrix4f;

Matrix4f matrix4f_vector_mult(const Matrix4f *m, const Vector4f *v);
Matrix4f matrix4f_matrix_mult(const Matrix4f *a, const Matrix4f *b);

void matrix4f_print(const Matrix4f *m);

#endif