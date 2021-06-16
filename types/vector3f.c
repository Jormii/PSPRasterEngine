#include "./vector3f.h"

#include <math.h>
#include <stdio.h>

float_psp vector3f_magnitude(const Vector3f *v)
{
    return sqrtf(vector3f_dot(v, v));
}

Vector3f *vector3f_normalize(Vector3f *v)
{
    float_psp magnitude = vector3f_magnitude(v);
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;

    return v;
}

Vector3f vector3f_normalized(const Vector3f *v)
{
    Vector3f copy = {v->x, v->y, v->z};
    vector3f_normalize(&copy);
    return copy;
}

float_psp vector3f_dot(const Vector3f *a, const Vector3f *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector3f vector3f_cross(const Vector3f *a, const Vector3f *b)
{
    return (Vector3f){
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x};
}

Vector3f vector3f_add(const Vector3f *a, const Vector3f *b)
{
    return (Vector3f){
        a->x + b->x,
        a->y + b->y,
        a->z + b->z};
}

Vector3f vector3f_substract(const Vector3f *a, const Vector3f *b)
{
    return (Vector3f){
        a->x - b->x,
        a->y - b->y,
        a->z - b->z};
}

Vector3f vector3f_multiply_scalar(const Vector3f *v, float_psp scalar)
{
    return (Vector3f){
        v->x * scalar,
        v->y * scalar,
        v->z * scalar};
}

Vector3f vector3f_divide_scalar(const Vector3f *v, float_psp scalar)
{
    return (Vector3f){
        v->x / scalar,
        v->y / scalar,
        v->z / scalar};
}

void vector3f_print(const Vector3f *v)
{
    printf("(%f, %f, %f)", v->x, v->y, v->z);
}