#ifndef VECTOR_3F_H
#define VECTOR_3F_H

#include "./types.h"

typedef struct Vector3f_t
{
    float_psp x;
    float_psp y;
    float_psp z;
} Vector3f;

float_psp vector3f_magnitude(const Vector3f *v);
Vector3f *vector3f_normalize(Vector3f *v);

float_psp vector3f_dot(const Vector3f *a, const Vector3f *b);
Vector3f vector3f_cross(const Vector3f *a, const Vector3f *b);

Vector3f vector3f_add(const Vector3f *a, const Vector3f *b);
Vector3f vector3f_substract(const Vector3f *a, const Vector3f *b);
Vector3f vector3f_multiply_scalar(const Vector3f *v, float_psp scalar);
Vector3f vector3f_divide_scalar(const Vector3f *v, float_psp scalar);

void vector3f_print(const Vector3f *v);

#endif