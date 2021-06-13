#ifndef VECTOR_4F_H
#define VECTOR_4F_H

#include "./types.h"

typedef struct Vector4f_t
{
    float_psp x;
    float_psp y;
    float_psp z;
    float_psp w;
} Vector4f;

float_psp vector4f_dot(const Vector4f *a, const Vector4f *b);

void vector4f_print(const Vector4f *v);

#endif