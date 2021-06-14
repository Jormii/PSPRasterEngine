#include "./vector3f.h"

#include <math.h>
#include <stdio.h>

float_psp vector3f_magnitude(const Vector3f *v)
{
    return sqrtf(vector3f_square_magnitude(v));
}

float_psp vector3f_square_magnitude(const Vector3f *v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

void vector3f_normalize(Vector3f *v)
{
    float_psp magnitude = vector3f_magnitude(v);
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}

void vector3f_normalized(const Vector3f *v, Vector3f *normal)
{
    normal->x = v->x;
    normal->y = v->y;
    normal->z = v->z;
    vector3f_normalize(normal);
}

float_psp vector3f_dot(const Vector3f *a, const Vector3f *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vector3f_cross(const Vector3f *a, const Vector3f *b, Vector3f *result)
{
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

void vector3f_add(const Vector3f *a, const Vector3f *b, Vector3f *result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void vector3f_substract(const Vector3f *a, const Vector3f *b, Vector3f *result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void vector3f_divide_scalar(const Vector3f *v, float_psp scalar, Vector3f *result)
{
    result->x = v->x / scalar;
    result->y = v->y / scalar;
    result->z = v->z / scalar;
}

void vector3f_print(const Vector3f *v)
{
    printf("(%f, %f, %f)", v->x, v->y, v->z);
}