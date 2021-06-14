#ifndef VECTOR_4F_H
#define VECTOR_4F_H

#include "./types.h"
#include "./vector3f.h"

typedef struct Vector4f_t
{
    float_psp x;
    float_psp y;
    float_psp z;
    float_psp w;
} Vector4f;

void vector4f_divide_by_w(const Vector4f *v, Vector3f *out);

float_psp vector4f_dot(const Vector4f *a, const Vector4f *b);

void vector4f_print(const Vector4f *v);

#endif