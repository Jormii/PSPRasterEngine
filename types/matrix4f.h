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

void matrix4f_vector_mult(const Matrix4f *m, const Vector4f *v, Vector4f *result);
void matrix4f_matrix_mult(const Matrix4f *a, const Matrix4f *b, Matrix4f *result);

void matrix4f_print(const Matrix4f *m);

#endif