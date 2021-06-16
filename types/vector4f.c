#include "./vector4f.h"

#include <stdio.h>

Vector3f vector4f_divide_by_w(const Vector4f *v)
{
    return (Vector3f){
        v->x / v->w,
        v->y / v->w,
        v->z / v->w};
}

float_psp vector4f_dot(const Vector4f *a, const Vector4f *b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z) + (a->w * b->w);
}

void vector4f_print(const Vector4f *v)
{
    printf("(%f, %f, %f, %f)", v->x, v->y, v->z, v->w);
}